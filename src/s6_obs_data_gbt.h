#ifndef _S6_OBS_DATA_GBT_H
#define _S6_OBS_DATA_GBT_H

#include "s6_databuf.h"

#define N_ADCS_PER_ROACH2 8     // should this be N_BEAM_SLOTS/2 ?

#define GBTSTATUS_STRING_SIZE 32
#define GBTSTATUS_BIG_STRING_SIZE 256

#define CURRENT_MJD ((time(NULL) / 86400.0 ) + 40587.0)             // 40587.0 is the MJD of the unix epoch

// idle status reasons bitmap
#define idle_nibble_01_1bit                     0x000000000000001; // saving these 4 for something important
#define idle_nibble_01_2bit                     0x000000000000002;
#define idle_nibble_01_4bit                     0x000000000000004;
#define idle_nibble_01_8bit                     0x000000000000008;

#define idle_atfctrmd_not_1                     0x000000000000010; // GB - ATFCTRMD Antenna,focusTrackingMode # if not 1 then data should be ignored
#define idle_atlpoaz1_too_large                 0x000000000000020; // GB - ATLPOAZ1 Antenna,localPointingOffsets,azOffset1 # radians - ignore data if too large
#define idle_atlpoaz2_too_large                 0x000000000000040; // GB - ATLPOAZ2 Antenna,localPointingOffsets,azOffset1 # radians - ignore data if too large
#define idle_atlpoel_too_large                  0x000000000000080; // GB - ATLPOEL Antenna,localPointingOffsets,elOffset # radians - ignore data if too large

#define idle_atlfcx_non_zero                    0x000000000000100; // GB - ATLFCX Antenna,local_focus_correction,X # mm - ignore if non-zero
#define idle_atlfcxt_non_zero                   0x000000000000200; // GB - ATLFCXT Antenna,local_focus_correction,Xt # subreflector tilt degrees - ignore if non-zero
#define idle_atlfcy_too_large                   0x000000000000400; // GB - ATLFCY Antenna,local_focus_correction,Y # mm - ignore if too large
#define idle_atlfctr_too_large                  0x000000000000800; // GB - ATLFCYT Antenna,local_focus_correction,Yt # subreflector tilt degrees- ignore if too large

#define idle_atlfcz_non_zero                    0x000000000001000; // GB - ATLFCZ Antenna,local_focus_correction,X # mm - ignore if non-zero
#define idle_atlfczt_non_zero                   0x000000000002000; // GB - ATLFCZT Antenna,local_focus_correction,Xt # subreflector tilt degrees - ignore if non-zero
#define idle_atoptmod_not_matched               0x000000000004000; // GB - ATOPTMOD Antenna,opticsMode # should match what IF manager reports
#define idle_atrecvr_not_matched                0x000000000008000; // GB - ATRECVR Antenna,receiver # should match IF manager, opticsMode, and GregorianReceiver

#define idle_atrxocta_wrong_degrees             0x000000000010000; // GB - ATRXOCTA Antenna,rxOpticsConfig,turretAngle # current rot angle of turrent in degrees (should match what's in TurretLocations)
#define idle_attrbeam_not_1                     0x000000000020000; // GB - ATTRBEAM Antenna,trackBeam  # if != 1 then using non central beam and data should be ignored
#define idle_atmfbs_wrong_state                 0x000000000040000; // GB - ATMFBS AntennaManager,feedBoomState # PF receiver && BOOM_EXTENDED || Gregorian && BOOM_RETRACTED
#define idle_atmtls_not_locked                  0x000000000080000; // GB - ATMTLS AntennaManager,turretLockState # If TURRET_LOCK_LOCKED, otherwise ignore data

#define idle_optgreg_not_true                   0x000000000100000; // GB - OPTGREG OpticsOK,Gregorian # if != TRUE then optics offset or tilted and sky pos and gain may be wrong
#define idle_optprime_not_true                  0x000000000200000; // GB - OPTPRIME OpticsOK,PrimeFocus # if != TRUE then optics offset or tilted and sky pos and gain may be wrong
#define idle_lo1fqsw_true                       0x000000000400000; // GB - LO1FQSW LO1,FrequencySwitching # if TRUE then data should probably be ignored
#define idle_lo1cfg_test_tone                   0x000000000800000; // GB - LO1CFG LO1,loConfig # if != (TrackA_BNotUsed || TrackB_ANotUsed) then good chance test tone injected

#define idle_lo1phcal_on                        0x000000001000000; // GB - LO1PHCAL LO1,phaseCalCtl # if ON the VLB phase cal is on and data should have "rail of lines"
#define idle_bammpwr1_bad_power                 0x000000002000000; // GB - BAMMPWR1 BankAMgr,measpwr1  # power levels in (dBn) of VEGAS samplers (polarization 1) - too different than -20 system may be non-linear
#define idle_bammpwr2_bad_power                 0x000000004000000; // GB - BAMMPWR2 BankAMgr,measpwr2  # power levels in (dBn) of VEGAS samplers (polarization 2) - too different than -20 system may be non-linear
#define idle_lastupdt_old                       0x000000008000000; // GB - last update to gbstatus is > 60 then something is wrong

#define idle_webcntrl_off                       0x000000010000000; // GB - webcontrol = 0, i.e. operators turned us off
#define idle_runalways_off                      0x000000020000000; // GB - runalways = 0
#define idle_nibble_08_4bit                     0x000000040000000;
#define idle_nibble_08_8bit                     0x000000080000000;

#define idle_nibble_09_1bit                     0x000000100000000;
#define idle_nibble_09_2bit                     0x000000200000000;
#define idle_nibble_09_4bit                     0x000000400000000;
#define idle_nibble_09_8bit                     0x000000800000000;

#define idle_nibble_10_1bit                     0x000001000000000;
#define idle_nibble_10_2bit                     0x000002000000000;
#define idle_nibble_10_4bit                     0x000004000000000;
#define idle_nibble_10_8bit                     0x000008000000000;

#define idle_nibble_11_1bit                     0x000010000000000;
#define idle_nibble_11_2bit                     0x000020000000000;
#define idle_nibble_11_4bit                     0x000040000000000;
#define idle_nibble_11_8bit                     0x000080000000000;

#define idle_nibble_12_1bit                     0x000100000000000;
#define idle_nibble_12_2bit                     0x000200000000000;
#define idle_nibble_12_4bit                     0x000400000000000;
#define idle_nibble_12_8bit                     0x000800000000000;

#define idle_nibble_13_1bit                     0x001000000000000;
#define idle_nibble_13_2bit                     0x002000000000000;
#define idle_nibble_13_4bit                     0x004000000000000;
#define idle_nibble_13_8bit                     0x008000000000000;

#define idle_nibble_14_1bit                     0x010000000000000;
#define idle_nibble_14_2bit                     0x020000000000000;
#define idle_nibble_14_4bit                     0x040000000000000;
#define idle_nibble_14_8bit                     0x080000000000000;

#define idle_nibble_15_1bit                     0x100000000000000;
#define idle_nibble_15_2bit                     0x200000000000000;
#define idle_nibble_15_4bit                     0x400000000000000;
#define idle_nibble_15_8bit                     0x800000000000000;

typedef struct gbtstatus {

   // actually from gbtstatus, i.e. the mysql database

   char LASTUPDT[GBTSTATUS_STRING_SIZE];       // last_update
   long LASTUPDTSTIME;
   char LST[GBTSTATUS_STRING_SIZE];            // lst
   long LSTSTIME;
   char UTC[GBTSTATUS_STRING_SIZE];            // utc
   long UTCSTIME;
   double MJD;                                 // mjd
   long MJDSTIME;
   char EPOCH[GBTSTATUS_STRING_SIZE];          // epoch
   long EPOCHSTIME;
   char MAJTYPE[GBTSTATUS_STRING_SIZE];        // major_type
   long MAJTYPESTIME;
   char MINTYPE[GBTSTATUS_STRING_SIZE];        // minor_type
   long MINTYPESTIME;
   char MAJOR[GBTSTATUS_STRING_SIZE];          // major
   long MAJORSTIME;
   char MINOR[GBTSTATUS_STRING_SIZE];          // minor
   long MINORSTIME;
   double AZCOMM;                              // az_commanded
   long AZCOMMSTIME;
   double ELCOMM;                              // el_commanded
   long ELCOMMSTIME;
   double AZACTUAL;                            // az_actual
   long AZACTUALSTIME;
   double ELACTUAL;                            // el_actual
   long ELACTUALSTIME;
   double AZERROR;                             // az_error
   long AZERRORSTIME;
   double ELERROR;                             // el_error
   long ELERRORSTIME;
   char LPCS[GBTSTATUS_STRING_SIZE];           // lpcs
   long LPCSSTIME;
   char FOCUSOFF[GBTSTATUS_STRING_SIZE];       // focus_offset
   long FOCUSOFFSTIME;
   char ANTMOT[GBTSTATUS_STRING_SIZE];         // ant_motion
   long ANTMOTSTIME;
   char RECEIVER[GBTSTATUS_STRING_SIZE];       // receiver
   long RECEIVERSTIME;
   double IFFRQ1ST;                            // first_if_freq
   long IFFRQ1STSTIME;
   double IFFRQRST;                            // if_rest_freq
   long IFFRQRSTSTIME;
   double DCRSCFRQ;                            // dcr_sky_center_freq (should be long?)
   long DCRSCFRQSTIME;
   double SPRCSFRQ;                            // spectral_processor_sky_freq (should be long?)
   long SPRCSFRQSTIME;
   double FREQ;                                // freq
   long FREQSTIME;
   char VELFRAME[GBTSTATUS_STRING_SIZE];       // velocity_frame
   long VELFRAMESTIME;
   char VELDEF[GBTSTATUS_STRING_SIZE];         // velocity_definition
   long VELDEFSTIME;
   double J2000MAJ;                            // j2000_major
   long J2000MAJSTIME;
   double J2000MIN;                            // j2000_minor
   long J2000MINSTIME;

   // the gbtstatus-derived fields below are from s6_observatory_gbt but not from gbtstatus/mysql
    
   double LSTH_DRV;         // lst in decimal hours
   long LSTH_DRVSTIME;
   double RA_DRV;           // RA in hours    taken from az/el actual and precessed to J2000
   long RA_DRVSTIME;
   double RADG_DRV;         // RA in degrees  taken from az/el actual and precessed to J2000
   long RADG_DRVSTIME;
   double DEC_DRV;          // DEC in degrees taken from az/el actual and precessed to J2000
   long DEC_DRVSTIME;

   // cleo server fields 

    char ATATCOTM[GBTSTATUS_STRING_SIZE]; // Antenna,Antenna,ConnectionToManager
    long ATATCOTMSTIME;
    double ATATCOTMMJD;
    char ATBEELOF[GBTSTATUS_STRING_SIZE]; // Antenna,beamElOffset
    long ATBEELOFSTIME;
    double ATBEELOFMJD;
    char ATBOT1EO[GBTSTATUS_STRING_SIZE]; // Antenna,beamOffsetTable,1,beamElOffset
    long ATBOT1EOSTIME;
    double ATBOT1EOMJD;
    char ATBOT1XO[GBTSTATUS_STRING_SIZE]; // Antenna,beamOffsetTable,1,beamXelOffset
    long ATBOT1XOSTIME;
    double ATBOT1XOMJD;
    char ATBOT1NM[GBTSTATUS_STRING_SIZE]; // Antenna,beamOffsetTable,1,name
    long ATBOT1NMSTIME;
    double ATBOT1NMMJD;
    char ATBOT1F1[GBTSTATUS_STRING_SIZE]; // Antenna,beamOffsetTable,1,SRFeed1
    long ATBOT1F1STIME;
    double ATBOT1F1MJD;
    char ATBOT1F2[GBTSTATUS_STRING_SIZE]; // Antenna,beamOffsetTable,1,SRFeed2
    long ATBOT1F2STIME;
    double ATBOT1F2MJD;
    char ATBEXLOF[GBTSTATUS_STRING_SIZE]; // Antenna,beamXelOffset
    long ATBEXLOFSTIME;
    double ATBEXLOFMJD;
    char ATFCTRMD[GBTSTATUS_STRING_SIZE]; // Antenna,focusTrackingMode
    long ATFCTRMDSTIME;
    double ATFCTRMDMJD;
    char ATGREGRC[GBTSTATUS_STRING_SIZE]; // Antenna,gregorianReceiver
    long ATGREGRCSTIME;
    double ATGREGRCMJD;
    char ATLFCX[GBTSTATUS_STRING_SIZE]; // Antenna,local_focus_correction,X
    long ATLFCXSTIME;
    double ATLFCXMJD;
    char ATLFCXT[GBTSTATUS_STRING_SIZE]; // Antenna,local_focus_correction,Xt
    long ATLFCXTSTIME;
    double ATLFCXTMJD;
    char ATLFCY[GBTSTATUS_STRING_SIZE]; // Antenna,local_focus_correction,Y
    long ATLFCYSTIME;
    double ATLFCYMJD;
    char ATLFCYT[GBTSTATUS_STRING_SIZE]; // Antenna,local_focus_correction,Yt
    long ATLFCYTSTIME;
    double ATLFCYTMJD;
    char ATLFCZ[GBTSTATUS_STRING_SIZE]; // Antenna,local_focus_correction,Z
    long ATLFCZSTIME;
    double ATLFCZMJD;
    char ATLFCZT[GBTSTATUS_STRING_SIZE]; // Antenna,local_focus_correction,Zt
    long ATLFCZTSTIME;
    double ATLFCZTMJD;
    char ATLPOAZ1[GBTSTATUS_STRING_SIZE]; // Antenna,localPointingOffsets,azOffset1
    long ATLPOAZ1STIME;
    double ATLPOAZ1MJD;
    char ATLPOAZ2[GBTSTATUS_STRING_SIZE]; // Antenna,localPointingOffsets,azOffset2
    long ATLPOAZ2STIME;
    double ATLPOAZ2MJD;
    char ATLPOEL[GBTSTATUS_STRING_SIZE]; // Antenna,localPointingOffsets,elOffset
    long ATLPOELSTIME;
    double ATLPOELMJD;
    char ATMATMCA[GBTSTATUS_STRING_SIZE]; // AntennaManager,AntennaManager,ConnectionToAccessor
    long ATMATMCASTIME;
    double ATMATMCAMJD;
    char ATMATMCM[GBTSTATUS_STRING_SIZE]; // AntennaManager,AntennaManager,ConnectionToManager
    long ATMATMCMSTIME;
    double ATMATMCMMJD;
    char ATMATMCT[GBTSTATUS_STRING_SIZE]; // AntennaManager,AntennaManager,ConnectionToTransporter
    long ATMATMCTSTIME;
    double ATMATMCTMJD;
    char ATMCDCJ2[GBTSTATUS_STRING_SIZE]; // AntennaManager,ccuData,DcJ2000,indicated
    long ATMCDCJ2STIME;
    double ATMCDCJ2MJD;
    char ATMCRAJ2[GBTSTATUS_STRING_SIZE]; // AntennaManager,ccuData,RaJ2000,indicated
    long ATMCRAJ2STIME;
    double ATMCRAJ2MJD;
    char ATMFBS[GBTSTATUS_STRING_SIZE]; // AntennaManager,feedBoomState
    long ATMFBSSTIME;
    double ATMFBSMJD;
    char ATMTLS[GBTSTATUS_STRING_SIZE]; // AntennaManager,turretLockState
    long ATMTLSSTIME;
    double ATMTLSMJD;
    char ATOPTMOD[GBTSTATUS_STRING_SIZE]; // Antenna,opticsMode
    long ATOPTMODSTIME;
    double ATOPTMODMJD;
    char ATRECVR[GBTSTATUS_STRING_SIZE]; // Antenna,receiver
    long ATRECVRSTIME;
    double ATRECVRMJD;
    char ATRXOCTA[GBTSTATUS_STRING_SIZE]; // Antenna,rxOpticsConfig,turretAngle
    long ATRXOCTASTIME;
    double ATRXOCTAMJD;
    char ATTRBEAM[GBTSTATUS_STRING_SIZE]; // Antenna,trackBeam
    long ATTRBEAMSTIME;
    double ATTRBEAMMJD;
    char ATTURLOC[GBTSTATUS_BIG_STRING_SIZE]; // Antenna,TurretLocations
    long ATTURLOCSTIME;
    double ATTURLOCMJD;
    char BAMBAMCA[GBTSTATUS_STRING_SIZE]; // BankAMgr,BankAMgr,ConnectionToAccessor
    long BAMBAMCASTIME;
    double BAMBAMCAMJD;
    char BAMBAMCM[GBTSTATUS_STRING_SIZE]; // BankAMgr,BankAMgr,ConnectionToManager
    long BAMBAMCMSTIME;
    double BAMBAMCMMJD;
    char BAMBAMCT[GBTSTATUS_STRING_SIZE]; // BankAMgr,BankAMgr,ConnectionToTransporter
    long BAMBAMCTSTIME;
    double BAMBAMCTMJD;
    char BAMMPWR1[GBTSTATUS_STRING_SIZE]; // BankAMgr,measpwr1
    long BAMMPWR1STIME;
    double BAMMPWR1MJD;
    char BAMMPWR2[GBTSTATUS_STRING_SIZE]; // BankAMgr,measpwr2
    long BAMMPWR2STIME;
    double BAMMPWR2MJD;
    char CLEOPID[GBTSTATUS_STRING_SIZE]; // CleoSetiServer,PID
    long CLEOPIDSTIME;
    double CLEOPIDMJD;
    char CLEOREV[GBTSTATUS_STRING_SIZE]; // CleoSetiServer,Revision
    long CLEOREVSTIME;
    double CLEOREVMJD;
    char IFMIFMCM[GBTSTATUS_STRING_SIZE]; // IFManager,IFManager,ConnectionToManager
    long IFMIFMCMSTIME;
    double IFMIFMCMMJD;
    char IFV1BW[GBTSTATUS_STRING_SIZE]; // IFManager,VEGAS_J1,bandwidth
    long IFV1BWSTIME;
    double IFV1BWMJD;
    char IFV1CSFQ[GBTSTATUS_STRING_SIZE]; // IFManager,VEGAS_J1,CorrectedSkyFrequency
    long IFV1CSFQSTIME;
    double IFV1CSFQMJD;
    char IFV1LVL[GBTSTATUS_STRING_SIZE]; // IFManager,VEGAS_J1,level
    long IFV1LVLSTIME;
    double IFV1LVLMJD;
    char IFV1IFFQ[GBTSTATUS_STRING_SIZE]; // IFManager,VEGAS_J1,spectrum,IF,centerFrequency
    long IFV1IFFQSTIME;
    double IFV1IFFQMJD;
    char IFV1SSB[GBTSTATUS_STRING_SIZE]; // IFManager,VEGAS_J1,spectrum,sideband
    long IFV1SSBSTIME;
    double IFV1SSBMJD;
    char IFV1SKFQ[GBTSTATUS_STRING_SIZE]; // IFManager,VEGAS_J1,spectrum,sky,centerFrequency
    long IFV1SKFQSTIME;
    double IFV1SKFQMJD;
    char IFV1TH[GBTSTATUS_STRING_SIZE]; // IFManager,VEGAS_J1,transform,1,horn
    long IFV1THSTIME;
    double IFV1THMJD;
    char IFV1TNCI[GBTSTATUS_STRING_SIZE]; // IFManager,VEGAS_J1,transform,1,name,circuit
    long IFV1TNCISTIME;
    double IFV1TNCIMJD;
    char IFV1TNCO[GBTSTATUS_STRING_SIZE]; // IFManager,VEGAS_J1,transform,1,name,component
    long IFV1TNCOSTIME;
    double IFV1TNCOMJD;
    char IFV1TT[GBTSTATUS_STRING_SIZE]; // IFManager,VEGAS_J1,transform,1,type
    long IFV1TTSTIME;
    double IFV1TTMJD;
    char IFV2BW[GBTSTATUS_STRING_SIZE]; // IFManager,VEGAS_J2,bandwidth
    long IFV2BWSTIME;
    double IFV2BWMJD;
    char IFV2CSFQ[GBTSTATUS_STRING_SIZE]; // IFManager,VEGAS_J2,CorrectedSkyFrequency
    long IFV2CSFQSTIME;
    double IFV2CSFQMJD;
    char IFV2LVL[GBTSTATUS_STRING_SIZE]; // IFManager,VEGAS_J2,level
    long IFV2LVLSTIME;
    double IFV2LVLMJD;
    char IFV2IFFQ[GBTSTATUS_STRING_SIZE]; // IFManager,VEGAS_J2,spectrum,IF,centerFrequency
    long IFV2IFFQSTIME;
    double IFV2IFFQMJD;
    char IFV2SSB[GBTSTATUS_STRING_SIZE]; // IFManager,VEGAS_J2,spectrum,sideband
    long IFV2SSBSTIME;
    double IFV2SSBMJD;
    char IFV2SKFQ[GBTSTATUS_STRING_SIZE]; // IFManager,VEGAS_J2,spectrum,sky,centerFrequency
    long IFV2SKFQSTIME;
    double IFV2SKFQMJD;
    char IFV2TH[GBTSTATUS_STRING_SIZE]; // IFManager,VEGAS_J2,transform,1,horn
    long IFV2THSTIME;
    double IFV2THMJD;
    char IFV2TNCI[GBTSTATUS_STRING_SIZE]; // IFManager,VEGAS_J2,transform,1,name,circuit
    long IFV2TNCISTIME;
    double IFV2TNCIMJD;
    char IFV2TNCO[GBTSTATUS_STRING_SIZE]; // IFManager,VEGAS_J2,transform,1,name,component
    long IFV2TNCOSTIME;
    double IFV2TNCOMJD;
    char IFV2TT[GBTSTATUS_STRING_SIZE]; // IFManager,VEGAS_J2,transform,1,type
    long IFV2TTSTIME;
    double IFV2TTMJD;
    char LO1ACA[GBTSTATUS_STRING_SIZE]; // LO1A,LO1A,ConnectionToAccessor
    long LO1ACASTIME;
    double LO1ACAMJD;
    char LO1ACM[GBTSTATUS_STRING_SIZE]; // LO1A,LO1A,ConnectionToManager
    long LO1ACMSTIME;
    double LO1ACMMJD;
    char LO1ACT[GBTSTATUS_STRING_SIZE]; // LO1A,LO1A,ConnectionToTransporter
    long LO1ACTSTIME;
    double LO1ACTMJD;
    char LO1APSFQ[GBTSTATUS_STRING_SIZE]; // LO1A,phaseState,frequency
    long LO1APSFQSTIME;
    double LO1APSFQMJD;
    char LO1BCA[GBTSTATUS_STRING_SIZE]; // LO1B,LO1B,ConnectionToAccessor
    long LO1BCASTIME;
    double LO1BCAMJD;
    char LO1BCM[GBTSTATUS_STRING_SIZE]; // LO1B,LO1B,ConnectionToManager
    long LO1BCMSTIME;
    double LO1BCMMJD;
    char LO1BCT[GBTSTATUS_STRING_SIZE]; // LO1B,LO1B,ConnectionToTransporter
    long LO1BCTSTIME;
    double LO1BCTMJD;
    char LO1BPSFQ[GBTSTATUS_STRING_SIZE]; // LO1B,phaseState,frequency
    long LO1BPSFQSTIME;
    double LO1BPSFQMJD;
    char LO1FQSW[GBTSTATUS_STRING_SIZE]; // LO1,FrequencySwitching
    long LO1FQSWSTIME;
    double LO1FQSWMJD;
    char LO1CM[GBTSTATUS_STRING_SIZE]; // LO1,LO1,ConnectionToManager
    long LO1CMSTIME;
    double LO1CMMJD;
    char LO1CFG[GBTSTATUS_STRING_SIZE]; // LO1,loConfig
    long LO1CFGSTIME;
    double LO1CFGMJD;
    char LO1PHCAL[GBTSTATUS_STRING_SIZE]; // LO1,phaseCalCtl
    long LO1PHCALSTIME;
    double LO1PHCALMJD;
    char OPTGREG[GBTSTATUS_STRING_SIZE]; // OpticsOK,Gregorian
    long OPTGREGSTIME;
    double OPTGREGMJD;
    char OPTPRIME[GBTSTATUS_STRING_SIZE]; // OpticsOK,PrimeFocus
    long OPTPRIMESTIME;
    double OPTPRIMEMJD;
    char SCPROJID[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,projectId
    long SCPROJIDSTIME;
    double SCPROJIDMJD;
    char SCSCCM[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,ScanCoordinator,ConnectionToManager
    long SCSCCMSTIME;
    double SCSCCMMJD;
    char SCSNUMBR[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,scanNumber
    long SCSNUMBRSTIME;
    double SCSNUMBRMJD;
    char SCSOURCE[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,source
    long SCSOURCESTIME;
    double SCSOURCEMJD;
    char SCSTATE[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,state
    long SCSTATESTIME;
    double SCSTATEMJD;
    char SCSACTSF[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,ActiveSurface
    long SCSACTSFSTIME;
    double SCSACTSFMJD;
    char SCSANAFR[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,AnalogFilterRack
    long SCSANAFRSTIME;
    double SCSANAFRMJD;
    char SCSANTEN[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,Antenna
    long SCSANTENSTIME;
    double SCSANTENMJD;
    char SCSARCHI[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,Archivist
    long SCSARCHISTIME;
    double SCSARCHIMJD;
    char SCSBCPM[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,BCPM
    long SCSBCPMSTIME;
    double SCSBCPMMJD;
    char SCSCCB26[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,CCB26_40
    long SCSCCB26STIME;
    double SCSCCB26MJD;
    char SCSCRACK[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,ConverterRack
    long SCSCRACKSTIME;
    double SCSCRACKMJD;
    char SCSDCR[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,DCR
    long SCSDCRSTIME;
    double SCSDCRMJD;
    char SCSGUPPI[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,GUPPI
    long SCSGUPPISTIME;
    double SCSGUPPIMJD;
    char SCSHOLOG[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,Holography
    long SCSHOLOGSTIME;
    double SCSHOLOGMJD;
    char SCSIFM[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,IFManager
    long SCSIFMSTIME;
    double SCSIFMMJD;
    char SCSIFR[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,IFRack
    long SCSIFRSTIME;
    double SCSIFRMJD;
    char SCSLO1[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,LO1
    long SCSLO1STIME;
    double SCSLO1MJD;
    char SCSMEASUR[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,Measurements
    long SCSMEASURSTIME;
    double SCSMEASURMJD;
    char SCSQUADRD[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,QuadrantDetector
    long SCSQUADRDSTIME;
    double SCSQUADRDMJD;
    char SCSR1[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,Rcvr1_2
    long SCSR1STIME;
    double SCSR1MJD;
    char SCSR12[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,Rcvr12_18
    long SCSR12STIME;
    double SCSR12MJD;
    char SCSR18[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,Rcvr18_26
    long SCSR18STIME;
    double SCSR18MJD;
    char SCSR2[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,Rcvr2_3
    long SCSR2STIME;
    double SCSR2MJD;
    char SCSR26[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,Rcvr26_40
    long SCSR26STIME;
    double SCSR26MJD;
    char SCSR40[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,Rcvr40_52
    long SCSR40STIME;
    double SCSR40MJD;
    char SCSR4[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,Rcvr4_6
    long SCSR4STIME;
    double SCSR4MJD;
    char SCSR68[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,Rcvr68_92
    long SCSR68STIME;
    double SCSR68MJD;
    char SCSR8[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,Rcvr8_10
    long SCSR8STIME;
    double SCSR8MJD;
    char SCSRA1[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,RcvrArray1_2
    long SCSRA1STIME;
    double SCSRA1MJD;
    char SCSRA18[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,RcvrArray18_26
    long SCSRA18STIME;
    double SCSRA18MJD;
    char SCSRMBA1[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,Rcvr_MBA1_5
    long SCSRMBA1STIME;
    double SCSRMBA1MJD;
    char SCSRPAR[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,Rcvr_PAR
    long SCSRPARSTIME;
    double SCSRPARMJD;
    char SCSRPF1[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,RcvrPF_1
    long SCSRPF1STIME;
    double SCSRPF1MJD;
    char SCSRPF2[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,RcvrPF_2
    long SCSRPF2STIME;
    double SCSRPF2MJD;
    char SCSSPROC[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,SpectralProcessor
    long SCSSPROCSTIME;
    double SCSSPROCMJD;
    char SCSSPECT[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,Spectrometer
    long SCSSPECTSTIME;
    double SCSSPECTMJD;
    char SCSSWSS[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,SwitchingSignalSelector
    long SCSSWSSSTIME;
    double SCSSWSSMJD;
    char SCSVEGAS[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,VEGAS
    long SCSVEGASSTIME;
    double SCSVEGASMJD;
    char SCSZPECT[GBTSTATUS_STRING_SIZE]; // ScanCoordinator,subsystemSelect,Zpectrometer
    long SCSZPECTSTIME;
    double SCSZPECTMJD;
    char VEGSFBW1[GBTSTATUS_STRING_SIZE]; // VEGAS,filter_bw,1
    long VEGSFBW1STIME;
    double VEGSFBW1MJD;
    char VEGSFBW2[GBTSTATUS_STRING_SIZE]; // VEGAS,filter_bw,2
    long VEGSFBW2STIME;
    double VEGSFBW2MJD;
    char VEGSFBW3[GBTSTATUS_STRING_SIZE]; // VEGAS,filter_bw,3
    long VEGSFBW3STIME;
    double VEGSFBW3MJD;
    char VEGSFBW4[GBTSTATUS_STRING_SIZE]; // VEGAS,filter_bw,4
    long VEGSFBW4STIME;
    double VEGSFBW4MJD;
    char VEGSFBW5[GBTSTATUS_STRING_SIZE]; // VEGAS,filter_bw,5
    long VEGSFBW5STIME;
    double VEGSFBW5MJD;
    char VEGSFBW6[GBTSTATUS_STRING_SIZE]; // VEGAS,filter_bw,6
    long VEGSFBW6STIME;
    double VEGSFBW6MJD;
    char VEGSFBW7[GBTSTATUS_STRING_SIZE]; // VEGAS,filter_bw,7
    long VEGSFBW7STIME;
    double VEGSFBW7MJD;
    char VEGSFBW8[GBTSTATUS_STRING_SIZE]; // VEGAS,filter_bw,8
    long VEGSFBW8STIME;
    double VEGSFBW8MJD;
    char VEGSSBAM[GBTSTATUS_STRING_SIZE]; // VEGAS,subsystemSelect,BankAMgr
    long VEGSSBAMSTIME;
    double VEGSSBAMMJD;
    char VEGSSBBM[GBTSTATUS_STRING_SIZE]; // VEGAS,subsystemSelect,BankBMgr
    long VEGSSBBMSTIME;
    double VEGSSBBMMJD;
    char VEGSSBCM[GBTSTATUS_STRING_SIZE]; // VEGAS,subsystemSelect,BankCMgr
    long VEGSSBCMSTIME;
    double VEGSSBCMMJD;
    char VEGSSBDM[GBTSTATUS_STRING_SIZE]; // VEGAS,subsystemSelect,BankDMgr
    long VEGSSBDMSTIME;
    double VEGSSBDMMJD;
    char VEGSSBEM[GBTSTATUS_STRING_SIZE]; // VEGAS,subsystemSelect,BankEMgr
    long VEGSSBEMSTIME;
    double VEGSSBEMMJD;
    char VEGSSBFM[GBTSTATUS_STRING_SIZE]; // VEGAS,subsystemSelect,BankFMgr
    long VEGSSBFMSTIME;
    double VEGSSBFMMJD;
    char VEGSSBGM[GBTSTATUS_STRING_SIZE]; // VEGAS,subsystemSelect,BankGMgr
    long VEGSSBGMSTIME;
    double VEGSSBGMMJD;
    char VEGSSBHM[GBTSTATUS_STRING_SIZE]; // VEGAS,subsystemSelect,BankHMgr
    long VEGSSBHMSTIME;
    double VEGSSBHMMJD;
    char VEGASCM[GBTSTATUS_STRING_SIZE]; // VEGAS,VEGAS,ConnectionToManager
    long VEGASCMSTIME;
    double VEGASCMMJD;

    int CLEOHELLO;                       // cleo server status.  1 = up, 0 = down 
    double CLEOMJD;                      // mjd of last cleao heart beat

   // the cleo-related derived fields below are from s6_observatory_gbt but not from cleo

   long LCUDSECS; // Last CLEO Update seconds (i.e. how many seconds since last known updated value from cleo
   long LCUDSECSSTIME;

   // idle status derived from all possible inputs 

   long IDLESTATUS; // 0 == we should take data, >0 we're idling due to set bit field values  
   long IDLESTATUSSTIME;   

   // the fields below are other scripts and not from gbtstatus/mysql/cleo

   int     coarse_chan_id;
   double  ADC0SDEV; 
   double  ADC1SDEV; 
   double  ADC0MEAN; 
   double  ADC1MEAN; 
   int     ADC0SDEVSTIME; 
   int     ADC1SDEVSTIME; 
   int     ADC0MEANSTIME; 
   int     ADC1MEANSTIME; 
   int     CLOCKTIM;
   double  CLOCKFRQ;
   double  CLOCKDBM;
   int     CLOCKLOC;
   int     BIRDITIM;
   double  BIRDIFRQ;
   double  BIRDIDBM;
   int     BIRDILOC;
   int     WEBCNTRL;    // 1 = on, 0 = off - for GBT operator control

} gbtstatus_t;

int get_obs_gbt_info_from_redis(gbtstatus_t *gbtstatus, char *hostname, int port);
int put_obs_gbt_info_to_redis(char * fits_filename, int instance, char *hostname, int port);

#endif  // _S6_OBS_DATA_GBT_H

