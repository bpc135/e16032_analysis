/* Correlator for Mg32 experiment
 * Based on DSSDCorrelator.cpp from Spectcl by S. Liddick
 * Yongchi Xiao, June, 2018
 * S. N. Liddick, June, 2018
 * B. P. Crider, June, 2018
 */

#include <RConfigure.h>
#include <math.h>
#include <iostream>
#include <iomanip>

#include "Parameters-ddas.h"
#include "Variables-ddas.h"
#include "TRandom.h"

#include "correlator.h"

#define CORRELATION 3

/****************************************
 * Correlation flags:                   *
 * ==================================== *
 *  4 - invalid pspmt implant pixel     *
 *  8 - invalid pspmt decay pixel       *
 * 12 - ion implantation                *
 * 16 - decay                           *
 * 20 - unknown                         *
 * 24 - invalid implant time            *
 * 28 - invalid decay time              *
 * 32 - exceeded correlation time       *
 * 36 - implant without pin (light ion) *
 * ==================================== *
 * 56 - reset correlation aray          * 
 ****************************************/

Correlator::Correlator() {
  cout << "Starting correlator..." << endl;
}

Correlator::~Correlator() {; }

void Correlator::Reset() {
  cout << "Why is array being reset ???????????????????? " <<endl;
  
  for (int i=1; i<17; i++) {
    for (int j=1; j<17; j++) {
      implant[i][j].implanted = false;
      implant[i][i].decayed = false;
      implant[i][j].numcorr = 0;
      implant[i][j].timehigh = 0;
      implant[i][j].timelow = 0;
      implant[i][j].timecfd = 0;
      implant[i][j].time = 0;
      implant[i][j].dE1 = 0;
      implant[i][j].dE2 = 0;
      implant[i][j].dE3 = 0;
      implant[i][j].isum = 0;
      implant[i][j].imax = 0;
      implant[i][j].tof = 0;
      implant[i][j].dt = 0;
      implant[i][j].dyecal = 0;
      implant[i][j].dyamp = 0;
      
      decay[i][j].dyamp = 0;
      decay[i][j].dyecal = 0;
      decay[i][j].timehigh = 0;
      decay[i][j].timelow = 0;
      decay[i][j].timecfd = 0;
      decay[i][j].time = 0;
      decay[i][j].dE1 = 0;
      decay[i][j].dE2 = 0;
      decay[i][j].dE3 = 0;
      decay[i][j].isum = 0;
      decay[i][j].imax = 0;
      decay[i][j].tof = 0;
      decay[i][j].dt = 0;
      decay[i][j].decaydt = 0;
    }
  }
}

double Correlator::Correlate(betadecay &bdecay, betadecayvariables &bdecayv,
			     int numX = 16, int numY = 16, int subseg = -1) {
  // reset correlator
  int resetCheck = -1;
  int condition = 0; 
  if(bdecayv.corr.reset != 0) {
    cout << "Resetting correlator array..." << endl;
    Correlator::Reset(); 
    resetCheck = 56; // ??
    bdecayv.corr.reset = 0; 
  }

  bool implanted = false;
  
  // get time
  double currentTime = 0;
  currentTime = (bdecay.clock.initial)/1.e6;  //time in ms 
  
  // signal flags
  bool hasPin = false;
  bool hasPin01 = false; 
  bool hasPspmt = false;
  bool hasSega = false;
  bool hasLabr3 = false;
  bool hasClyc = false;
  bool overflowPSPMT = false;
  // event flags
  bool isImplant = false;
  bool isDecay = false;
  bool isLightIon = false;
  bool isSegaOnly = false;
  bool isLabr3Only = false;
  bool isClycOnly = false;
  
  // pspmt
  int implantX = -1;
  int implantY = -1;
  int decayX = -1;
  int decayY = -1;
  int xPos = -1;
  int yPos = -1; 
  double pspmtTime = -1; 

  // pin
  double pin1Energy = -1;
  double pin2Energy = -1;
  double pin3Energy = -1; 
  double pin1Time = -1;
  double pin2Time = -1;
  double pin3Time = -1; 

  // sega
  double segaTime = -1;
  int segaMult = bdecayv.hit.sega; 

  // labr3
  double labr3Time = -1;
  int labr3Mult = bdecayv.hit.labr3; 

  // cylc
  double clycTime = -1;
  double clycEnergy = -1;

  double outval = -1; 

  /*********************************
   *** Event Type Identification ***
   *********************************/
  if(bdecayv.hit.pin01 > 0 || bdecayv.hit.pin02 > 0 || bdecayv.hit.pin03 > 0)
    hasPin = true;
  if(bdecayv.hit.pin01 == 1) hasPin01 = true; // low gain events
  if(bdecayv.hit.pspmt == 1) hasPspmt = true; 
  if(segaMult > 0) hasSega = true;
  if(labr3Mult > 0) hasLabr3 = true;
  if(bdecayv.hit.clyc > 0) hasClyc = true;
  if(bdecay.pspmt.dyoverflow == 1) overflowPSPMT = true; 
  // advanced type identification 
  // if(hasPin01 && hasPspmt) {
  //   isImplant = true;
  // }
  if(hasPin01 && hasPspmt && bdecay.pin01.ecal > 9000) {
    isImplant = true;
  }
  //also putting a cut on dynode energy to throw out what appear to be
  //light ions (this hopefully is just a temporary fix since we
  //lose ~20% of ions right off the bat
  if(!hasPin && hasPspmt && !overflowPSPMT && bdecay.pspmt.dyE1real_single < 9200) {
    isDecay = true; 
  }
  //CHANGE THIS after PIN03 gain increased
  if(!hasPin && hasPspmt && overflowPSPMT) {
    isLightIon = true; 
  }
  //
  if(!hasPin && !hasPspmt && hasSega && !hasLabr3 && !hasClyc) {
    isSegaOnly = true; 
  }
  if(!hasPin && !hasPspmt && !hasSega && hasLabr3 && !hasClyc) {
    isLabr3Only = true;
  }
  if(!hasPin && !hasPspmt && !hasSega && !hasLabr3 && hasClyc) {
    isClycOnly = true;
  }
  
  
  /*******************************
   *****     CORRELATIONS    *****
   *******************************/


  /************ PSPMT:ION-DECAY ************/
  if(hasPspmt) {
    pspmtTime = currentTime;
    
    bool goodPos = false; 
    // process different types of Pspmt events
    if(isImplant) {
      // get position info., from low gain info.
      // xPos = bdecay.pspmt.aampmaxx;//(int)((bdecay.pspmt.loposxEcent50-1)*2.0 + 0.5) +1;
      // yPos  = bdecay.pspmt.aampmaxy;//(int)((bdecay.pspmt.loposyEcent50-1)*2.0 + 0.5) +1;

      //xPos = bdecay.pspmt.aminimpx;//(int)((bdecay.pspmt.loposxEcent50-1)*2.0 + 0.5) +1;
      //yPos  = bdecay.pspmt.aminimpy;//(int)((bdecay.pspmt.loposyEcent50-1)*2.0 + 0.5) +1;

      xPos = bdecay.pspmt.xpos;
      yPos = bdecay.pspmt.ypos;
      
      // cout << "IMPLANT array " << xPos << " " << yPos << " " << currentTime << " " << bdecay.pin01.energy <<endl;
      // for(int y = 0; y<16; y++){
      //     for(int x= 0; x<16; x++){
      //       cout << std::setw(10) << implant[x][y].time / 1000000. << " " ;//" (" <<  std::setw(7) << tdiff[x][y][m] << ")";
      //     }
      //     cout << endl;
      // }

      // cout << endl;

      
       
      // sanity check on position
      if(xPos < 17 && xPos > 0
	 && yPos < 17 && yPos > 0) {
	goodPos = true;
	condition = 12; 
      }
      if(!goodPos) condition = 4;
      else { // valid position
        
	implant[xPos][yPos].implanted = true; 
	// check existing implant
	double timeDiffIon = -1;
	
	if(implant[xPos][yPos].time >= 0) {
	  timeDiffIon = currentTime - implant[xPos][yPos].time;
	  if(timeDiffIon > 0) {
	    // assign values 
	    implant[xPos][yPos].time = currentTime;
	    implant[xPos][yPos].tof = bdecay.pid.i2stof;
	    // anode and dynode
	    implant[xPos][yPos].dyecal = bdecay.pspmt.dyecal;
	    implant[xPos][yPos].dyamp = bdecay.pspmt.dyamp;
	    implant[xPos][yPos].asum = bdecay.pspmt.asum; // evaluated in unpacker.cpp
	    // 
	    implant[xPos][yPos].dE1 = bdecay.pid.de1;
	    implant[xPos][yPos].dE2 = bdecay.pid.de2;
	    implant[xPos][yPos].dE3 = bdecay.pid.de3;
	    implant[xPos][yPos].isum = bdecay.pid.isum;
	    implant[xPos][yPos].imax = bdecay.pid.imax;
	    implant[xPos][yPos].dt = timeDiffIon;
	    //
	    implant[xPos][yPos].numcorr = 0;
	    
	    // ions
	    bdecay.corr.dtimplant = implant[xPos][yPos].dt;
	    bdecay.corr.itime = implant[xPos][yPos].time;
	    bdecay.corr.ide1  = implant[xPos][yPos].dE1;
	    bdecay.corr.ide2  = implant[xPos][yPos].dE2;
	    bdecay.corr.ide3  = implant[xPos][yPos].dE3;
	    bdecay.corr.iisum = implant[xPos][yPos].isum;
	    bdecay.corr.iimax = implant[xPos][yPos].imax;
	    bdecay.corr.itof  = implant[xPos][yPos].tof;
	    bdecay.corr.iasum = implant[xPos][yPos].asum;
	    bdecay.corr.idyecal = implant[xPos][yPos].dyecal;
	    bdecay.corr.idyamp = implant[xPos][yPos].dyamp;


	    // cout << "UPDATED IMPLANT array " << endl;
	    // for(int y = 0; y<16; y++){
	    //   for(int x= 0; x<16; x++){
	    // 	cout << std::setw(10) << implant[x][y].time /1000000. << " " ;//" (" <<  std::setw(7) << tdiff[x][y][m] << ")";
	    //   }
	    //   cout << endl;
	    // }
	    
	    // cout << endl;


	    
	  } else condition = 24; // bad implant time; 
	} else timeDiffIon = -1; 
      } // end:if(goodPos)
    }else if(isDecay) {
      // get position info., from high gain info.
      //xPos = bdecay.pspmt.amaxx;//(int)((bdecay.pspmt.posxEcent-1)*2.0 + 0.5)+1;
      //yPos  = bdecay.pspmt.amaxy;//(int)((bdecay.pspmt.posyEcent-1)*2.0 + 0.5)+1;
      xPos = bdecay.pspmt.xpos;
      yPos = bdecay.pspmt.ypos;
      //cout << "DECAY INFO " << xPos << " " << yPos << " " << currentTime << endl;
      //if(currentTime > 535.357) exit(1);
      

      //search around position for correlation
      double decay_pixel[CORRELATION][CORRELATION];
      double decay_pixel_pick = 1000000.0; // one thousand seconds (units of ms)
      
      // reset the temporary array with very large times
      for (int i=0; i<CORRELATION; i++) {
	for (int j=0; j<CORRELATION; j++) {
	  decay_pixel[i][j] = 1000000000.;//DBL_MAX;
	}
      }
      
      int corr_limit = (CORRELATION - 1)/2;   // for CORRELATION=3, corr_limt=1
      int num_corr = 0; 
      
      for (int i=(-corr_limit); i<(corr_limit+1); i++) {
	for (int j=(-corr_limit); j<(corr_limit+1); j++) {
	  if (((xPos+i) >= 0) && ((xPos+i) < 16)
	      && ((yPos+j) >= 0) && ((yPos+j) < 16)) { // Valid pixel? 
	    
	    if (implant[xPos+i][yPos+j].implanted) {
	      if(currentTime-implant[xPos+i][yPos+j].time > 0){
		decay_pixel[i+corr_limit][j+corr_limit] =currentTime-implant[xPos+i][yPos+j].time;
	      }
	      implanted = true;
	      num_corr++;
	    }
	    
	  }
	}
      }
      
      int tempxpos = 500;
      int tempypos = 500;

      // cout << "TEMP CORR array " << xPos << " " << yPos << " " <<endl;
      // if ( (xPos!=500 && yPos !=500)){
      // 	cout <<  currentTime -  implant[xPos][yPos].time << endl;
      // 	for(int y = 0; y<7; y++){
      //     for(int x= 0; x<7; x++){
      //       cout << std::setw(11) << decay_pixel[x][y] << " " ;//" (" <<  std::setw(7) << tdiff[x][y][m] << ")";
      //     }
      //     cout << endl;
      // 	}
      // }
      // cout << endl;  
      
      
      if (implanted) {
	for (int i=(-corr_limit); i<(corr_limit+1); i++) {
	  for (int j=(-corr_limit); j<(corr_limit+1); j++) {
	    if (((xPos+i) >= 0) && ((xPos+i) < 16)
		&& ((yPos+j) >= 0) && ((yPos+j) < 16)) { // was giving invalid pixels, so we'll try adding this condition here too
	      
	      if (decay_pixel[i+corr_limit][j+corr_limit] < decay_pixel_pick && ( decay_pixel[i+corr_limit][j+corr_limit] >0 ) ) {
		decay_pixel_pick = decay_pixel[i+corr_limit][j+corr_limit];
		tempxpos = xPos+i;
		tempypos = yPos+j;
		
		//cout << "back: " << back << " front: " << front << endl;
		
	      }
	      
	    }
	  }
	}
      }
	
      bdecay.corr.mult = (long)num_corr;
      
      num_corr = 0;
      
      xPos = tempxpos;
      yPos = tempypos;


      // cout << "DECAY array " << xPos << " " << yPos << " " <<endl;
      // if ( (xPos!=500 && yPos !=500)) cout <<  currentTime - implant[xPos][yPos].time << endl;
      // for(int y = 0; y<16; y++){
      //     for(int x= 0; x<16; x++){
      //       cout << std::setw(7) << currentTime - implant[x][y].time << " " ;//" (" <<  std::setw(7) << tdiff[x][y][m] << ")";
      //     }
      //     cout << endl;
      // }

      // cout << endl;
      
      
      // sanity check on position
      if(xPos < 17 && xPos > 0
	 && yPos < 17 && yPos > 0) {
	goodPos = true;
	condition = 16; 
      }      
      if(!goodPos) condition = 8;
      else { // valid decay position 
	// condition for time interval
	const double tDiffCorrCut = 1000; // correlation time in ms
	double ionDecayTDiff = currentTime - implant[xPos][yPos].time;
	if(ionDecayTDiff < 0) condition = 28;
	else if(ionDecayTDiff < tDiffCorrCut/*decay[xPos][yPos].time < tDiffCorrCut*/) { // decay found and correlated 
		//		&& implant[xPos][yPos].time > 0)
		//		&& implant[xPos][yPos].implanted == true) {
	  outval = ionDecayTDiff; 
	  // assign values, sanity check neglected here
	  decay[xPos][yPos].time = currentTime;
	  // dynode and anode
	  decay[xPos][yPos].asum = bdecay.pspmt.asum;
	  decay[xPos][yPos].dyecal = bdecay.pspmt.dyecal;
	  decay[xPos][yPos].dyamp = bdecay.pspmt.dyamp;
	  // related to ions
	  decay[xPos][yPos].dt   = ionDecayTDiff;
	  decay[xPos][yPos].dE1  = implant[xPos][yPos].dE1;
	  decay[xPos][yPos].dE2  = implant[xPos][yPos].dE2;
	  //decay[xPos][yPos].dE3  = implant[xPos][yPos].dt; 
	  //
	  decay[xPos][yPos].isum = implant[xPos][yPos].isum;
	  decay[xPos][yPos].imax = implant[xPos][yPos].imax;
	  decay[xPos][yPos].tof  = implant[xPos][yPos].tof;
	  //decay[xPos][yPos].idyecal  = implant[xPos][yPos].dyecal;
	  //decay[xPos][yPos].idyamp  = implant[xPos][yPos].dyamp;
	  // decay[xPos][yPos].decaydt = decaydt;
	  decay[xPos][yPos].dnumcorr++;
	  /*if(ionDecayTDiff > tDiffCorrCut)*/
	  condition = 32;
	  //else { // reasonable time diff.
	  /* add to correlation recorder */
	  // decays
	  bdecay.corr.dDeltaTIon = implant[xPos][yPos].dt;
	  bdecay.corr.dtimecal = decay[xPos][yPos].dt*bdecayv.clock.calib;
	  bdecay.corr.dtime    = decay[xPos][yPos].time;
	  bdecay.corr.dde1     = decay[xPos][yPos].dE1;
	  bdecay.corr.dde2     = decay[xPos][yPos].dE2;
	  bdecay.corr.dde3     = decay[xPos][yPos].dE3;
	  bdecay.corr.disum    = decay[xPos][yPos].isum;
	  bdecay.corr.dimax    = decay[xPos][yPos].imax;
	  bdecay.corr.dyecal   = decay[xPos][yPos].dyecal;
	  bdecay.corr.dyamp   = decay[xPos][yPos].dyamp;
	  bdecay.corr.didyecal   = implant[xPos][yPos].dyecal;
	  bdecay.corr.didyecal   = implant[xPos][yPos].dyecal;
	  bdecay.corr.dtof     = decay[xPos][yPos].tof;
	  bdecay.corr.dnumcorr = decay[xPos][yPos].dnumcorr;
	  // pos. info.
	  bdecay.corr.implantX = xPos;
	  bdecay.corr.implantY = yPos;
	  bdecay.corr.decayX = xPos;
	  bdecay.corr.decayY = yPos;


	    //} // end:if(good correlation time)
	} // end:if(decay found)
	//else condition=32;
      } // end:if(goodPos)
    } // end:if(isDecay)
  } /* end:if(hasPspmt) */ else if(isSegaOnly) {  
    /* Correlation between SeGA and CeBr3
     */
    const double isomerTimeCut = 0.04; // in ms, = 40 us
    segaTime = currentTime; 
    // check ion implantation
    for(int i = 0; i < 17; i++) {
      for(int j = 0; j < 17; j++) {
	bool goodTimeDiff = false; 
	if(implant[i][j].time > 0) {// found an ion
	  double isomerTDiff = segaTime - implant[i][j].time;
	  if(isomerTDiff > 0. && isomerTDiff < isomerTimeCut) {
	    // good time diff.
	    goodTimeDiff = true; 
	  }
	  if(goodTimeDiff) {
	    // add to correlation recorder
	    bdecay.corr.gtimecal = isomerTDiff*1.e3; //?
	    bdecay.corr.gde1 = (long)implant[i][j].dE1;
	    bdecay.corr.gde2 = (long)implant[i][j].dE2;
	    bdecay.corr.gde3 = (long)implant[i][j].dE3;
	    bdecay.corr.gisum = (long)implant[i][j].isum;
	    bdecay.corr.gimax = (long)implant[i][j].imax;
	    bdecay.corr.gtof = (long)implant[i][j].tof;
	  }
	}
      }
    } // end:loop over PSPMT
  } /* end:isSegaOnly */ else if(isLabr3Only) {
    /* Correlation between LaBr3 and CeBr3
     */
    const double promptTimeCut = 0.04; // in ms, = 40 us
    segaTime = currentTime; 
    // check ion implantation
    for(int i = 0; i < 17; i++) {
      for(int j = 0; j < 17; j++) {
	bool goodTimeDiff = false; 
	if(implant[i][j].time > 0) {// found an ion
	  double promptTDiff = segaTime - implant[i][j].time;
	  if(promptTDiff > 0. && promptTDiff < promptTimeCut) {
	    // good time diff.
	    goodTimeDiff = true; 
	  }
	  if(goodTimeDiff) {
	    // add to correlation recorder
	    bdecay.corr.ltimecal = promptTDiff*1.e3; //?
	    bdecay.corr.lde1 = (long)implant[i][j].dE1;
	    bdecay.corr.lde2 = (long)implant[i][j].dE2;
	    bdecay.corr.lde3 = (long)implant[i][j].dE3;
	    bdecay.corr.lisum = (long)implant[i][j].isum;
	    bdecay.corr.limax = (long)implant[i][j].imax;
	    bdecay.corr.ltof = (long)implant[i][j].tof;
	  }
	}
      }
    } // end:loop over PSPMT
  } /* end:isLabr3Only */ else if(isClycOnly) {
    /* Correlation between CYLC and CeBr3
     */
    const double clycTimeCut = 0.04; // in ms, = 40 us
    segaTime = currentTime; 
    // check ion implantation
    for(int i = 0; i < 17; i++) {
      for(int j = 0; j < 17; j++) {
	bool goodTimeDiff = false; 
	if(implant[i][j].time > 0) {// found an ion
	  double clycTDiff = segaTime - implant[i][j].time;
	  if(clycTDiff > 0. && clycTDiff < clycTimeCut) {
	    // good time diff.
	    goodTimeDiff = true; 
	  }
	  if(goodTimeDiff) {
	    // add to correlation recorder
	    bdecay.corr.ctimecal = clycTDiff*1.e3; //?
	    bdecay.corr.cde1 = (long)implant[i][j].dE1;
	    bdecay.corr.cde2 = (long)implant[i][j].dE2;
	    bdecay.corr.cde3 = (long)implant[i][j].dE3;
	    bdecay.corr.cisum = (long)implant[i][j].isum;
	    bdecay.corr.cimax = (long)implant[i][j].imax;
	    bdecay.corr.ctof = (long)implant[i][j].tof;
	  }
	}
      }
    } // end:loop over PSPMT    
  }

  /* end correlation, copied */
  //if(resetCheck != 0) condition = resetCheck;
  bdecay.corr.flag = (long) condition;

  return outval; 
  
}


