/* s6_write_etfits.c */
#define _ISOC99_SOURCE  // For long double strtold
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <hiredis.h>
#include "s6_databuf.h"
//#include "s6GPU.h"
#include "s6_obs_data.h"
#include "s6_etfits.h"

#define DEBUGOUT 0

//----------------------------------------------------------
int write_etfits(s6_output_databuf_t *db, int block_idx, etfits_t *etf, int nhits, scram_t *scram_p) {
//----------------------------------------------------------
    int row, rv;
    int nchan, nivals, nsubband;
    char* temp_str;
    double temp_dbl;

    int * status_p = &(etf->status);
    *status_p = 0;

    scram_t scram;

    // Create the initial file or change to a new one if needed.
    if (etf->new_file || (etf->multifile==1 && etf->rownum > etf->rows_per_file))
    {
        if (!etf->new_file) {
            printf("Closing file '%s'\n", etf->filename);
            fits_close_file(etf->fptr, status_p);
        }
        etfits_create(etf);
        if(*status_p) {
            fprintf(stderr, "Error creating/initializing new etfits file.\n");
            fits_report_error(stderr, *status_p);
            exit(1);
        }    
        // TODO this could be done once per run rather than once per file
        // TODO some (all?) of these will come from iput parms (status shmem)
        // TODO update code versions
        etf->primary_hdr.n_subband = N_COARSE_CHAN;
        etf->primary_hdr.n_chan    = N_FINE_CHAN;
        etf->primary_hdr.n_inputs  = N_BEAMS * N_POLS_PER_BEAM;
        // TODO not yet implemented
        //etf->primary_hdr.bandwidth = ;
        //etf->primary_hdr.chan_bandwidth = ;
        //etf->primary_hdr.freq_res = ;
        write_primary_header(etf);
    }

    // populate hits header data
    // TODO maybe I should do away with this and write directly to the header
    //      from sram in write_hits_header()
    for(int i=0; i < N_BEAMS*N_POLS_PER_BEAM; i++) {
        etf->hits_hdr[i].time    = scram_p->AGCTIME;     // TODO is this right?   
        etf->hits_hdr[i].ra      = scram_p->ra_by_beam[int(floor(i/N_POLS_PER_BEAM))];       
        etf->hits_hdr[i].dec     = scram_p->dec_by_beam[int(floor(i/N_POLS_PER_BEAM))];  
        etf->hits_hdr[i].beampol = i;       
    }

    if(! *status_p) write_integration_header(etf, scram_p);

    if(! *status_p) write_hits(db, block_idx, etf, nhits);

#if 1
    // Now update some key values if no CFITSIO errors
    if (! *status_p) {
        etf->rownum   += nhits;     // TODO do I need these?
        etf->tot_rows += nhits;
        etf->N += 1;
        //etf->T += dcols->exposure;
    }
#endif

    if(*status_p) {
        fprintf(stderr, "FITS error, exiting.\n");
        exit(1);
    }

    return *status_p;
}

//----------------------------------------------------------
int etfits_create(etfits_t * etf) {
//----------------------------------------------------------
    int * status_p = &(etf->status);
    *status_p = 0;

    // TODO enclose all init code in a do-as-needed block
    // Initialize the key variables if needed
    if (etf->new_file == 1) {  // first time writing to the file
        etf->status = 0;
        etf->tot_rows = 0;
        etf->N = 0L;
        etf->T = 0.0;
        etf->mode = 'w';

        // Create the output directory if needed
        char datadir[1024];
        strncpy(datadir, etf->basefilename, 1023);
        char *last_slash = strrchr(datadir, '/');
        if (last_slash!=NULL && last_slash!=datadir) {
            *last_slash = '\0';
            printf("Using directory '%s' for output.\n", datadir);
            char cmd[1024];
            sprintf(cmd, "mkdir -m 1777 -p %s", datadir);
            system(cmd);
        }
        etf->new_file = 0;
    }   // end first time writing to the file
    etf->filenum++;
    etf->rownum = 1;

    sprintf(etf->filename, "%s_%04d.fits", etf->basefilename, etf->filenum);

    // Create basic FITS file from our template
    char *s6_dir = getenv("S6_DIR");
    char template_file[1024];
    if (s6_dir==NULL) {
        s6_dir = (char *)".";
        fprintf(stderr, 
                "Warning: S6_DIR environment variable not set, using current directory for ETFITS template.\n");
    }
    printf("Opening file '%s'\n", etf->filename);
    sprintf(template_file, "%s/%s", s6_dir, ETFITS_TEMPLATE);
    if(! *status_p) fits_create_template(&(etf->fptr), etf->filename, template_file, status_p);

    // Check to see if file was successfully created
    if (*status_p) {
        fprintf(stderr, "Error creating sdfits file from template.\n");
        fits_report_error(stderr, *status_p);
    }

    return *status_p;
}

//----------------------------------------------------------
int etfits_close(etfits_t *etf) {
//----------------------------------------------------------
    int * status_p = &(etf->status);
    *status_p = 0;

fprintf(stderr, "in close, status is %d\n", *status_p);
    if (! *status_p) {
        fits_close_file(etf->fptr, status_p);
        printf("Closing file '%s'\n", etf->filename);
    }
    printf("Done.  %s %d data rows (%f sec) in %d files (status = %d).\n",
            etf->mode=='r' ? "Read" : "Wrote", 
            etf->tot_rows, etf->T, etf->filenum, *status_p);

    return *status_p;
}

//----------------------------------------------------------
int write_primary_header(etfits_t * etf) {
//----------------------------------------------------------
    int * status_p = &(etf->status);
    *status_p = 0;

    int itmp;
    char ctmp[40];


fprintf(stderr, "writing primary header\n");

    if(! *status_p) fits_get_system_time(ctmp, &itmp, status_p);      // date the file was written

    if(! *status_p) fits_movabs_hdu(etf->fptr, 1, NULL, status_p);    // go to primary HDU

    if(! *status_p) fits_update_key(etf->fptr, TSTRING, "DATE",     ctmp,                            NULL, status_p);
    if(! *status_p) fits_update_key(etf->fptr, TINT,    "NSUBBAND", &(etf->primary_hdr.n_subband),   NULL, status_p); 
    if(! *status_p) fits_update_key(etf->fptr, TINT,    "NCHAN",    &(etf->primary_hdr.n_chan),      NULL, status_p); 
    if(! *status_p) fits_update_key(etf->fptr, TINT,    "NINPUTS",  &(etf->primary_hdr.n_inputs),    NULL, status_p); 
    // TODO not yet implemented
    //if(! *status_p) fits_update_key(etf->fptr, TINT,    "BANDWID",  &(etf->primary_hdr.bandwidth),       NULL, status_p); 
    //if(! *status_p) fits_update_key(etf->fptr, TINT,    "CHAN_BW",  &(etf->primary_hdr.chan_bandwidth),  NULL, status_p); 
    //if(! *status_p) its_update_key(etf->fptr, TINT,    "FREQRES",  &(etf->primary_hdr.freq_res),        NULL, status_p);    // redundant w/ CHAN_BW? 

    if (*status_p) {
        fprintf(stderr, "Error updating primary header.\n");
        fits_report_error(stderr, *status_p);
    }

    return *status_p;
}

//----------------------------------------------------------
int write_integration_header(etfits_t * etf, scram_t *scram) {
//----------------------------------------------------------
    
    int * status_p = &(etf->status);
    *status_p = 0;

    static int first_time=1;
    
    *status_p = 0;
fprintf(stderr, "writing integration header\n");
    if(first_time) {
        // go to the template created HDU
        if(! status_p) fits_movnam_hdu(etf->fptr, BINARY_TBL, (char *)"AOSCRAM", 0, status_p);
        first_time = 0;
    } else {
        // create new HDU
        if(! *status_p) fits_create_tbl(etf->fptr, BINARY_TBL, 0, 0, NULL, NULL, NULL, (char *)"AOSCRAM", status_p);
    }

    if(! *status_p) fits_update_key(etf->fptr, TSTRING,  "EXTNAME", (char *)"AOSCRAM", NULL, status_p); 
    // observatory (scram) data 
    if(! *status_p) fits_update_key(etf->fptr, TINT,    "PNTSTIME",  &(scram->PNTSTIME), NULL, status_p); 
    if(! *status_p) fits_update_key(etf->fptr, TDOUBLE, "PNTRA",     &(scram->PNTRA),    NULL, status_p);      
    if(! *status_p) fits_update_key(etf->fptr, TDOUBLE, "PNTDEC",    &(scram->PNTDEC),   NULL, status_p);      
    if(! *status_p) fits_update_key(etf->fptr, TDOUBLE, "PNTMJD",    &(scram->PNTMJD),   NULL, status_p);      

    if(! *status_p) fits_update_key(etf->fptr, TINT,    "AGCSTIME", &(scram->AGCSTIME),  NULL, status_p); 
    if(! *status_p) fits_update_key(etf->fptr, TDOUBLE, "AGCAZ",    &(scram->AGCAZ),     NULL, status_p); 
    if(! *status_p) fits_update_key(etf->fptr, TDOUBLE, "AGCZA",    &(scram->AGCZA),     NULL, status_p); 
    if(! *status_p) fits_update_key(etf->fptr, TINT,    "AGCTIME",  &(scram->AGCTIME),   NULL, status_p); 
    if(! *status_p) fits_update_key(etf->fptr, TDOUBLE, "AGCLST",   &(scram->AGCLST),    NULL, status_p);     // TODO

    if(! *status_p) fits_update_key(etf->fptr, TINT,    "ALFSTIME", &(scram->ALFSTIME),  NULL, status_p); 
    if(! *status_p) fits_update_key(etf->fptr, TINT,    "ALFBIAS1", &(scram->ALFBIAS1),  NULL, status_p); 
    if(! *status_p) fits_update_key(etf->fptr, TINT,    "ALFBIAS2", &(scram->ALFBIAS2),  NULL, status_p); 
    if(! *status_p) fits_update_key(etf->fptr, TDOUBLE, "ALFMOPOS", &(scram->ALFMOPOS),  NULL, status_p);

    if(! *status_p) fits_update_key(etf->fptr, TINT,    "IF1STIME", &(scram->IF1STIME),  NULL, status_p); 
    if(! *status_p) fits_update_key(etf->fptr, TDOUBLE, "IF1SYNHZ", &(scram->IF1SYNHZ),  NULL, status_p); 
    if(! *status_p) fits_update_key(etf->fptr, TINT,    "IF1SYNDB", &(scram->IF1SYNDB),  NULL, status_p); 
    if(! *status_p) fits_update_key(etf->fptr, TDOUBLE, "IF1RFFRQ", &(scram->IF1RFFRQ),  NULL, status_p); 
    if(! *status_p) fits_update_key(etf->fptr, TDOUBLE, "IF1IFFRQ", &(scram->IF1IFFRQ),  NULL, status_p);
    if(! *status_p) fits_update_key(etf->fptr, TINT,    "IF1ALFFB", &(scram->IF1ALFFB),  NULL, status_p);

    if(! *status_p) fits_update_key(etf->fptr, TINT,    "IF2STIME", &(scram->IF2STIME),  NULL, status_p); 
    if(! *status_p) fits_update_key(etf->fptr, TINT,    "IF2ALFON", &(scram->IF2ALFON),  NULL, status_p);

    if(! *status_p) fits_update_key(etf->fptr, TINT,    "TTSTIME",  &(scram->TTSTIME),   NULL, status_p); 
    if(! *status_p) fits_update_key(etf->fptr, TINT,    "TTTURENC", &(scram->TTTURENC),  NULL, status_p); 
    if(! *status_p) fits_update_key(etf->fptr, TDOUBLE, "TTTURDEG", &(scram->TTTURDEG),  NULL, status_p);
#if 0
    fits_update_key(etf->fptr, TINT,    "MISSEDPK", &(scram.MISSEDPK),    NULL, status_p);    // missed packets per input per second 
#endif

    if (*status_p) {
        fprintf(stderr, "Error writing integration header.\n");
        fits_report_error(stderr, *status_p);
    }

    return *status_p;
}

//----------------------------------------------------------
int write_hits_header(etfits_t * etf, int beampol) {
//----------------------------------------------------------
// TODO have to know what input (beam/pol) this is

#define TFIELDS 4
    int * status_p = &(etf->status);
    *status_p = 0;

    static int first_time=1;

    int tbltype                = BINARY_TBL;
    long long naxis2           = 0;
    //const int tfields          = 3;
    // TODO check chan types!
    const char *ttype[TFIELDS] = {"DETPOW  ", "MEANPOW ",  "COARCHAN", "FINECHAN"};
    const char *tform[TFIELDS] = {"1E",       "1E",        "1I",        "1J"};     // cfitsio datatype codes 
    if(first_time) {
        // go to the template created HDU
        if(! *status_p) fits_movnam_hdu(etf->fptr, BINARY_TBL, (char *)"ETHITS", 0, status_p);
        first_time = 0;
    } else {
        // create new HDU
        if(! *status_p) fits_create_tbl(etf->fptr, BINARY_TBL, 0, TFIELDS, (char **)&ttype, (char **)&tform, NULL, (char *)"ETHITS", status_p);
    }
    if(! *status_p) fits_update_key(etf->fptr, TINT,    "TIME",    &(etf->hits_hdr[beampol].time),    NULL, status_p);    
    if(! *status_p) fits_update_key(etf->fptr, TDOUBLE, "RA",      &(etf->hits_hdr[beampol].ra),      NULL, status_p);   
    if(! *status_p) fits_update_key(etf->fptr, TDOUBLE, "DEC",     &(etf->hits_hdr[beampol].dec),     NULL, status_p);   
    if(! *status_p) fits_update_key(etf->fptr, TINT,    "BEAMPOL", &(etf->hits_hdr[beampol].beampol), NULL, status_p);   

    if (*status_p) {
        fprintf(stderr, "Error writing hits header.\n");
        fits_report_error(stderr, *status_p);
    }
}

//----------------------------------------------------------
int write_hits(s6_output_databuf_t *db, int block_idx, etfits_t *etf, int nhits) {      
//----------------------------------------------------------

    long nrows, firstrow, firstelem, nelements, colnum;
    int hit_i, nhits_this_input;  // TODO should these be longs or unsigned?
    int cur_beam, cur_input, cur_beampol;  // TODO should these be longs or unsigned?
    static int first_time=1;

    int * status_p = &(etf->status);
    *status_p = 0;

fprintf(stderr, "writing hits\n");
    //std::vector<hits_t> hits;

    std::vector<float> det_pow;
    std::vector<float> mean_pow;
    std::vector<int>   coarse_chan;
    std::vector<int>   fine_chan;
    float* det_pow_p;
    float* mean_pow_p;
    int* coarse_chan_p;
    int* fine_chan_p;

    firstrow  = 1;
    firstelem = 1;

    hit_i = 0;              
    while(hit_i < nhits) {
fprintf(stderr, "hit_i %d nhits %d\n", hit_i, nhits);
        // init for first / next input
        det_pow.clear();
        mean_pow.clear();
        coarse_chan.clear();
        fine_chan.clear();
        nhits_this_input = 0;

        // calculate hits header fields and populate the header 
        cur_beam    = db->block[block_idx].hits[hit_i].beam;
        cur_input   = db->block[block_idx].hits[hit_i].input;
        cur_beampol = cur_beam * N_POLS_PER_BEAM + cur_input;
fprintf(stderr, "writing header for beam %d input %d beampol %d\n", cur_beam, cur_input, etf->hits_hdr[cur_beam].beampol);
        etf->hits_hdr[cur_beampol].beampol = cur_beampol;
        // TODO populate missed packets
        write_hits_header(etf, cur_beampol);

        // separate the data columns for this input
        while(db->block[block_idx].hits[hit_i].input == cur_input && hit_i < nhits) {
            det_pow.push_back(db->block[block_idx].hits[hit_i].power);
            mean_pow.push_back(db->block[block_idx].hits[hit_i].baseline);
            coarse_chan.push_back(db->block[block_idx].hits[hit_i].coarse_chan);
            fine_chan.push_back(db->block[block_idx].hits[hit_i].fine_chan);
            nhits_this_input++;
            hit_i++;
        }
        // hit_i should now reference next input or one past all inputs
fprintf(stderr, "det_pow.size %ld nhits_this_input %d\n", det_pow.size(), nhits_this_input);

        // write the hits for this input
        det_pow_p     = &det_pow[0];
        mean_pow_p    = &mean_pow[0];
        coarse_chan_p = &coarse_chan[0];
        fine_chan_p   = &fine_chan[0];
        colnum      = 1;
        if(! *status_p) fits_write_col(etf->fptr, TFLOAT, colnum, firstrow, firstelem, nhits_this_input, det_pow_p, status_p);
        colnum      = 2;
        if(! *status_p) fits_write_col(etf->fptr, TFLOAT, colnum, firstrow, firstelem, nhits_this_input, mean_pow_p, status_p);
        colnum      = 3;
        if(! *status_p) fits_write_col(etf->fptr, TINT, colnum, firstrow, firstelem, nhits_this_input, coarse_chan_p, status_p);
        colnum      = 4;
        if(! *status_p) fits_write_col(etf->fptr, TINT, colnum, firstrow, firstelem, nhits_this_input, fine_chan_p, status_p);
    }  // end while hit_i < nhits

    if (*status_p) {
        fprintf(stderr, "Error writing hits.\n");
        fits_report_error(stderr, *status_p);
    }

    return *status_p;
}