/*      File:  Variables.h
    	Variable definitions for the beta-decay setup
*/

#ifndef VARIABLESDDAS_H
#define VARIABLESDDAS_H

// Clock Calibrator
class bdecayvClockCalibrator
{  
 public:
  float calib;
  float scale;
  float max;
  float max_daughter;
  
 public:
  bdecayvClockCalibrator();
  ~bdecayvClockCalibrator();
  void Initialize();
};

// Pin Calibrator

class bdecayvPinCalibrator 
{
 public:
  float slope;
  float intercept;
  float thresh;
  float ithresh;
  float iuld;

 public:
  bdecayvPinCalibrator();
  ~bdecayvPinCalibrator();
  void Initialize();
};

// TAC Calibrator

class bdecayvTacCalibrator
{
 public:
  float i2ncorr;
  float i2ncorrpos;
  float i2scorr;
  float i2scorrpos;
  float pin01corr1;
  float pin01corr2;
  float pin01corrpos;

  float i2scorr_offset;
  float pin01corr;
  float pin01corr_offset;
  float pin02corr;
  float pin02corr_offset;

 public:
  bdecayvTacCalibrator();
  ~bdecayvTacCalibrator();
  void Initialize();
};

// DSSD Calibrator

class bdecayvDSSDCalibrator
{
 public:
  float hioffset[17];
  float hislope[17];
  float loslope[17];

  float hipulseareaoffset[17];
  float hipulseareaslope[17];

  float hipulsefitoffset[17];
  float hipulsefitslope[17];

 public:
  bdecayvDSSDCalibrator();
  ~bdecayvDSSDCalibrator();
  void Initialize();
};

// DSSD Threshold

class bdecayvDSSDThresh
{
 public:
  float hithresh[17];
  float lothresh[17];
  float loduld[17];
  float duld[17];

 public:
  bdecayvDSSDThresh();
  ~bdecayvDSSDThresh();
  void Initialize();
};

// SSSD Calibration

class bdecayvSSSDCalibrator
{
 public:
  float offset[17];
  float slope[17];
  float thresh[17];
  float uld[17];

 public:
  bdecayvSSSDCalibrator();
  ~bdecayvSSSDCalibrator();
  void Initialize();
};

// Hit flags

class bdecayvHitFlag 
{
 public:
  int pin01;
  int pin02;
  int pin03;
  int pin04;
  int ge;
  int fronthi;
  int frontlo;
  int frontloall;
  int backhi;
  int backlo;
  int backloall;
  int sega;
  int labr3;
  int clyc;
  int pspmt;
  int i2n;
  int i2s;
  int dynode;
  
 public:
  bdecayvHitFlag();
  ~bdecayvHitFlag();
  void Initialize();
};

// Correlation

class bdecayvCorrelation 
{
 public:
  int reset;
  int resetclock; // Reset 50MHz clock
  float minimplant;

 public:
  bdecayvCorrelation();
  ~bdecayvCorrelation();
  void Initialize();
};

// PID

class bdecayvPidCalibrator
{
 public:
  float de1_scale;
  float de1_offset;
  float de1_tof_corr;
  float de1_tof_offset;
  float de2_scale;
  float de2_offset;
  float de2_tof_corr;
  float de2_tof_offset;
  float de3_scale;
  float de3_offset;
  float de3_tof_corr;
  float de3_tof_offset;
  float de4_scale;
  float de4_offset;
  float de4_tof_corr;
  float de4_tof_offset;
  float isum_scale;
  float isum_offset;
  float isum_tof_corr;
  float isum_tof_offset;
  float imax_scale;
  float imax_offset;
  float imax_tof_corr;
  float imax_tof_offset;

 public:
  bdecayvPidCalibrator();
  ~bdecayvPidCalibrator();
  void Initialize();
};

// PID

class bdecayvSegaCalibrator
{
 public:
  float square[37];
  float slope[37];
  float intercept[37];
  float thresh[37];
  float hithresh[37];
  float lotime;
  float hitime;

 public:
  bdecayvSegaCalibrator();
  ~bdecayvSegaCalibrator();
  void Initialize();
};

// PSPMT Calibrator

class bdecayvPSPMTCalibrator
{
 public:
  double offset[258];
  double slope[258];
  double thresh[258];
  double ithresh[258];
  double uld[258];
  double time_deviation[258]; 
  
 public:
  bdecayvPSPMTCalibrator();
  ~bdecayvPSPMTCalibrator();
  void Initialize();
};

// I2 Calibrator

class bdecayvI2Calibrator 
{
 public:
  float slope;
  float intercept;
  float thresh;

 public:
  bdecayvI2Calibrator();
  ~bdecayvI2Calibrator();
  void Initialize();
};


// LaBr3
class bdecayvLaBr3
{
 public:
  float square[17];
  float slope[17];
  float intercept[17];
  float toffset[17];
  float dynode_toffset[17];
  float thresh[37];
  float hithresh[37];
  float lotime;
  float hitime;
  float labr_toffset[17][17];

 public:
  bdecayvLaBr3();
  ~bdecayvLaBr3();
  void Initialize();
};

// CLYC
class bdecayvCLYC
{
 public:
  float square[17];
  float slope[17];
  float intercept[17];
  float toffset[17];
  float dynode_toffset[17];
  float thresh[37];
  float hithresh[37];
  float lotime;
  float hitime;
  float clyc_toffset[17][17];

 public:
  bdecayvCLYC();
  ~bdecayvCLYC();
  void Initialize();
};



/* The entire beta decay setup */
class betadecayvariables
{
 public:
  bdecayvClockCalibrator clock;
  bdecayvClockCalibrator clockisomer;
  bdecayvPinCalibrator pin01;
  bdecayvPinCalibrator pin02;
  bdecayvPinCalibrator pin03;
  bdecayvPinCalibrator pin04;
  bdecayvTacCalibrator tac;
  bdecayvHitFlag hit;
  bdecayvCorrelation corr;
  bdecayvPidCalibrator pid;
  bdecayvSegaCalibrator sega;
  bdecayvPSPMTCalibrator pspmt;
  bdecayvI2Calibrator i2n;
  bdecayvI2Calibrator i2s;
  bdecayvLaBr3 labr3;
  bdecayvCLYC clyc;

 public:
  betadecayvariables();
  ~betadecayvariables();

  void Initialize();
  void ReadSega(char *Name);
  void ReadOther(char *Name);
  void ReadLaBr3(char *Name);
  void ReadCLYC(char *Name);
  void ReadPSPMT(char *Name);
  void ReadPSPMT_TimeDev(char *Name);

};

#endif
