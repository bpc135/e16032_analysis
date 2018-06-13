/*****************************************************************/
/* Correlator.cpp for DSSD Beta Decay - Digital Data Acquisition */
/* Based on standard BCS SpecTcl codes                           */
/* Heather Crawford/Sean Liddick, June 2010                      */
/*****************************************************************/

/********************************************/
/* Correlation flags:                       */
/*                                          */
/*   4 - invalid DSSD implant pixel         */
/*   8 - invalid DSSD decay pixel           */
/*  12 - implantation event                 */
/*  16 - decay event                        */
/*  20 - unknown event                      */
/*  24 - front, no back decay               */
/*  28 - back, no front decay               */
/*  32 - back-2-back implantations          */
/*  36 - back-2-back decays                 */
/*  40 - invalid implant time               */
/*  44 - invalid decay time                 */
/*  48 - exceeded correlation time          */
/*  52 - long-lived implant rejection       */
/*  56 - reset correlation array            */
/*  72 - no implant                         */
/*  80 - implant not 70Co (test purpose)    */
/********************************************/

#include <RConfigure.h>
#include <math.h>
#include <iostream>
#include <iomanip>

#include "Parameters-ddas.h"
#include "Variables-ddas.h"
#include "correlator.h"
#include "TRandom.h"

//#define CORRELATION 1
#define CORRELATION 3

Correlator::Correlator() {
  cout << "Starting correlator..." << endl;
}

Correlator::~Correlator() { ; }

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


 //Cut for 70Co
  c70Co = new TCutG("c70Co",14);
  c70Co->SetTitle("c70Co");
  c70Co->SetVarX("bdecay.pid.i2ntof");
  c70Co->SetVarY("bdecay.pid.de1");
 
  c70Co->SetPoint(0,8800.484,12769);
  c70Co->SetPoint(1,8719.542,12790.63);
  c70Co->SetPoint(2,8613.038,12790.63);
  c70Co->SetPoint(3,8515.055,12747.38);
  c70Co->SetPoint(4,8451.153,12668.08);
  c70Co->SetPoint(5,8442.632,12559.96);
  c70Co->SetPoint(6,8515.055,12473.46);
  c70Co->SetPoint(7,8625.819,12415.79);
  c70Co->SetPoint(8,8766.403,12365.33);
  c70Co->SetPoint(9,8885.687,12329.29);
  c70Co->SetPoint(10,9017.752,12314.88);
  c70Co->SetPoint(11,9149.816,12358.13);
  c70Co->SetPoint(12,9176.907,12463);
  c70Co->SetPoint(13,9149.816,12552.75);
  c70Co->SetPoint(14,9051.833,12682.5);
  c70Co->SetPoint(15,8978.764,12715.29);
  c70Co->SetPoint(16,8894.208,12740.17);
  c70Co->SetPoint(17,8800.484,12769);

}


double Correlator::Correlate(betadecay &bdecay, betadecayvariables &bdecayv, 
			     int nstripx = 16, int nstripy = 16, 
			     int subseg = -1) {



  int lostripx = 1,  histripx = 1 + nstripx;
  int lostripy = 1,  histripy = 1 + nstripy;

  double outval = -1.;  int returnval = 0;

  int i, j;
  int numcorr = -1;
  int condition = 0;
  int resetcheck = 0;
  
  int ibackch = 100, ifrontch = 100;
  int ifrontmult = 0, ibackmult = 0;
  int dbackch = 100, dfrontch = 100;

  bool implant_event = false;
  bool implant_tof = false;
  bool decay_event = false;
  bool sega_only = false;
  bool unknown_event = false;
  bool fhitdc_reject = false;
  bool decayenergy = false;
  
  bool implanted = false;

  if (bdecayv.corr.reset != 0) {
    cout << "Resetting correlator array..." << endl;
    Correlator::Reset();
    resetcheck = 56; /* Condition flag will be set to 56 when reset done */
    bdecayv.corr.reset = 0;
  }
  
  /*** Implant/Decay Logic ***/
  
  /*
    Pixel location is defined by the front and back strips
    showing the maximum energy deposited (in implant or decay)
    in each side of the DSSD.  The strip array runs from 1 to 40.  
    If there is no event data, imaxch and dmaxch are set to 100;
    there are channel checks to reject events with invalid positions.
  */
  
  /* First establish pixel locations for implants/decays */
  /* Implants first... */


  //8 x 8 
  //  ifrontch = (int)bdecay.pspmt.loposxEcent;//(bdecay.front.imaxch);  
  //  ibackch  = (int)bdecay.pspmt.loposyEcent;//(bdecay.back.imaxch);

  //16 x 16
  //rounding for the position pixel
  ifrontch = (int)((bdecay.pspmt.loposxEcent-1)*2.0 + 0.5)+1;//(bdecay.front.imaxch);  
  ibackch  = (int)((bdecay.pspmt.loposyEcent-1)*2.0 + 0.5)+1;//(bdecay.back.imaxch);
    
  /* Now decays... */

  //8 x 8 
  // dfrontch = (int)bdecay.pspmt.posxEcent;//bdecay.front.dmaxch;
  // dbackch  = (int)bdecay.pspmt.posyEcent;//bdecay.back.dmaxch;
  
  //16 x 16
  //rounding for the position pixel
  dfrontch = (int)((bdecay.pspmt.posxEcent-1)*2.0 + 0.5)+1;//bdecay.front.dmaxch;
  dbackch  = (int)((bdecay.pspmt.posyEcent-1)*2.0 + 0.5)+1;//bdecay.back.dmaxch;

  /* Now establish boolean results of simple requirements to determine
     implants vs. decays vs. oddball events */
  
  /* Low-gain events in PIN1, PIN2, DSSD - implant */
  if ((bdecayv.hit.pin01 == 1) && bdecay.pspmt.dyamp > 0) {
    implant_event = true;
    returnval += 2;
  } else {
    implant_event = false;
  }
  
  /* High-gain events with no PIN1, PIN2 signals - decays */
  if ( (bdecayv.hit.pin01 == 0 && bdecayv.hit.pin02 == 0 && bdecayv.hit.pin03 == 0 && bdecay.pspmt.posxEcent > 0 && bdecay.pspmt.posyEcent > 0 && bdecay.pspmt.dyamp > 0) /*&& (bdecay.pspmt.dynode>0 || bdecay.pspmt.dynodeTDiff >0))*/) {
    decay_event = true;
    returnval += 16;
  }
  
  /* SeGA 'only' events - correlated SeGA with implants for isomers*/
  if ((bdecayv.hit.pin01 == 0) && (bdecayv.hit.pin02 ==0) && bdecay.pspmt.dyamp <= 0) {
    sega_only = true;
    returnval += 64;
  }

  /* Construct 48-bit current time */
  double current_time = 0.;
  current_time = (bdecay.clock.initial) / 1000000.;

  // if (implant_event && ifrontch==8 && ibackch == 8) {
  //  printf("Correlator(): IMPLANT F,B = %d %d\n", ifrontch, ibackch);
  //  cout << "TIME: " << current_time << endl;
  // }

  // if (decay_event && dfrontch == 8 && dbackch == 8 && implant[8][8].implanted) {
  //  printf("Correlator(): DECAY F,B = %d %d\n", dfrontch, dbackch);
  //  cout << "TIME: " << current_time << endl;
  // }
   
  if (implant_event) {

    //if(c70Co->IsInside(bdecay.pid.i2ntof,bdecay.pid.de1)) {
      
    /* Check for valid implantation pixel in DSSD */
    if ((ibackch >= lostripx) && (ibackch <= histripx) && 
	(ifrontch >= lostripy) && (ifrontch <= histripy)) {
	
      condition = 12; /* Good implantation */
      returnval += 1024;
	
	
      // cout<<"implant: "<<ibackch<<"  "<<ifrontch<<endl;
      // cout<<"implanted?: "<< implant[ibackch][ifrontch].implanted <<"  decayed?: "<<implant[ibackch][ifrontch].decayed<<endl;
      // cout<<"condition: "<<condition<<"  ";
	
      //Check for back to back implants (not like we can do anything about it)
      if (implant[ibackch][ifrontch].implanted) {
	if(implant[ibackch][ifrontch].decayed) {
	  implant[ibackch][ifrontch].decayed = false;
	}
	else {
	  condition = 32; /* Back-2-back implantation in DSSD with no decay in between*/
	}
      }
      else { 
	//make sure the implanted flag is true regardless
	implant[ibackch][ifrontch].implanted = true;
	implant[ibackch][ifrontch].decayed = false;
	
      }
	
      // cout<<condition<<endl;
      
      // /* Check for existing implant in DSSD pixel */
      // if (implant[ibackch][ifrontch].implanted) {
      // 	condition = 32; /* Back-2-back implantation in DSSD */
      // } else {
      // 	implant[ibackch][ifrontch].implanted = true;
      // }
      
      // Changed snl, time between implants should always be calculated
      double implant_time = 0;
      
      /* Get full clock time stamp of last implantation */
      implant_time = implant[ibackch][ifrontch].time;
      
      if (implant_time > current_time) {
	condition = 40;
      }
      double deltaT;
      deltaT = current_time - implant_time;
      implant[ibackch][ifrontch].dt = deltaT; /* 1ms per bin */
      
      //Adding exclusion zone.  Dont want to skew the time distributions 
      //4000 for 68Ni, 900 for 68Co
      //ONLY USE THIS FOR FITTING DECAY CURVES... FOR GAMMA RAYS TAKE OUT
      //if(deltaT>900.0) {
	
        /* Update time and tof */    
        implant[ibackch][ifrontch].time = current_time;
        implant[ibackch][ifrontch].timelow = bdecay.clock.fast;
        implant[ibackch][ifrontch].timehigh = bdecay.clock.slow;
        implant[ibackch][ifrontch].timecfd = bdecay.clock.cfd;
        implant[ibackch][ifrontch].tof = bdecay.pid.i2ntof;//bdecay.pid.xfptof;
	
      	/* Update dE from PINs, etc. */
      	implant[ibackch][ifrontch].dE1 = bdecay.pid.de1;
      	implant[ibackch][ifrontch].dE2 = bdecay.pid.de2;
      	implant[ibackch][ifrontch].dE3 = bdecay.pid.de3;
      	implant[ibackch][ifrontch].isum = bdecay.pid.isum;
      	implant[ibackch][ifrontch].imax = bdecay.pid.imax;
	
      	/* Reset counter */
      	implant[ibackch][ifrontch].numcorr = 0;  
	
      	bdecay.corr.dtimplant = implant[ibackch][ifrontch].dt;
      	bdecay.corr.itime = implant[ibackch][ifrontch].time;
      	bdecay.corr.ide1  = (long)implant[ibackch][ifrontch].dE1;
      	bdecay.corr.ide2  = (long)implant[ibackch][ifrontch].dE2;
      	bdecay.corr.ide3  = (long)implant[ibackch][ifrontch].dE3;
      	bdecay.corr.iisum = (long)implant[ibackch][ifrontch].isum;
      	bdecay.corr.iimax = (long)implant[ibackch][ifrontch].imax;
      	bdecay.corr.itof  = (long)implant[ibackch][ifrontch].tof;
	
      	// cout<<condition<<endl<<endl;
	
      	if(bdecay.corr.dtimplant < 0 && 0) {
      	  cout << "Corr(): Neg. implant time" << deltaT << " " 
      	       << bdecay.corr.dtimplant << " " << current_time << " " 
      	       << implant_time << " " << bdecay.clock.fast << " " 
      	       << bdecay.clock.slow << " " 
      	       << implant[ibackch][ifrontch].timelow << " " 
      	       << implant[ibackch][ifrontch].timehigh << endl;
      	}
	//} //end exclusion zone condition
    } else {
      condition = 4; /* Invalid DSSD implantation pixel */
    }
    //}
    // else {
    //   condition = 80;   //70Co check
    // }
  } else if (decay_event) {

    //   cout<<"decay: "<<dbackch<<"  "<<dfrontch<<endl;

    //  cout<<"decay"<<endl;
    
    // if (dfrontch == 8 && dbackch == 8 && implant[8][8].implanted) 
    //  cout << "I BELIEVE I AM A DECAY " << endl;

    // bdecay.corr.dlongRise = bdecay.back.longRise;
    //bdecay.corr.ddoubleTrigger = bdecay.back.doubleTrigger;

    returnval += 2048;
    
    /* Code will search over user-defined correlation first to 
       find decay event in DSSD */
    
    int num_corr = 0;
    
    /* Search over the nearest pixels to find the most recent implant
       and the correlate with it. */
    int corr_limit = (CORRELATION - 1)/2;
    int flolimit = dfrontch - corr_limit;
    int fhilimit = dfrontch + corr_limit;
    int blolimit = dbackch - corr_limit;
    int bhilimit = dbackch + corr_limit;
    
    int front = 500;
    int back = 500;
    
    double tdiffold = 100000000000;

    for(int k = flolimit; k < fhilimit+1; k++){
      for(int l = blolimit; l < bhilimit+1; l++){
	if( (k >= lostripx) && (k<=histripx) && (l>=lostripy) && (l<=histripy)){
	  if(implant[l][k].implanted){
	    implanted = true; /* Set implanted flag */
	    /* Determine time between now and previous implant */
	    double tmp_implanttime = (implant[l][k].time);
	    double tdiff = current_time - tmp_implanttime;
	    if((tdiff < tdiffold) && (tdiff > 0)){ /* Take the most 
						      recent implant */
	      num_corr++; /* Number of implants in the field */
	      tdiffold = tdiff;
	      front = k;
	      back = l;
	      //if(dfrontch == front) { bdecay.corr.field = 1; }
	      //else { bdecay.corr.field = 3; }
	      //if(dbackch == back) { bdecay.corr.field = 1; }
	      //else { bdecay.corr.field = 3; }
	    }
	  }
	}
      }
    }

    // if (dfrontch == 8 && dbackch == 8 && implant[8][8].implanted) 
    //cout << "Decay OK: F B " << front << " " << back << endl;
    
    if (implanted) {
      bdecay.corr.mult = (long)num_corr;
      
      /* Set decay locations */
      bdecay.corr.dfrontch = dfrontch;
      bdecay.corr.dbackch  = dbackch;
      bdecay.corr.frontch  = front;
      bdecay.corr.backch   = back;
      
      /* Check for valid decay (really correlated implant) pixel in DSSD */
      if ( (back >= lostripy) && (back <= histripy) && 
	   (front >= lostripx) && (front <= histripx) ) {
	
	condition = 16; /* Good decay flag */

	if (implant[back][front].implanted) {
	  double implant_time;
	  implant_time = 0;
	  /* Figure out the implantation time */
	  implant_time = (implant[back][front].time);
	  
	  /* Determine the last decay time in this pixel */
	  if (implant_time > current_time) { condition = 44; }
	  
	  double deltaT;
	  deltaT = current_time - implant_time; /* in 1ms bins */	  

	  // if (dfrontch == 8 && dbackch == 8 && implant[8][8].implanted) 
	  //cout << "DECAY STILL OK deltaT " << deltaT << endl;

	  /* Make sure time between back-to-back implants is long; 
	     if not, reject event */

	  if (((implant[back][front].dt) >= 
	       bdecayv.corr.minimplant*bdecayv.clock.scale) || 
	      (implant[back][front].dt == 0)) {
	    
	    /* Do not process correlations that extend 
	       beyond a fixed time */	    
	    if ((deltaT < bdecayv.clock.max) && (deltaT >= 0) ) {	      
	      decay[back][front].time = current_time;
	      decay[back][front].timelow = bdecay.clock.fast;
	      decay[back][front].timehigh = bdecay.clock.slow;
	      decay[back][front].timecfd = bdecay.clock.cfd;
	      decay[back][front].dt   = deltaT;
	      decay[back][front].dE1  = implant[back][front].dE1;
	      decay[back][front].dE2  = implant[back][front].dE2;
	      decay[back][front].dE3  = implant[back][front].dt; 
	      /* dE3 take over variable for time btwn imp */
	      decay[back][front].isum = implant[back][front].isum;
	      decay[back][front].imax = implant[back][front].imax;
	      decay[back][front].tof  = implant[back][front].tof;
	      // decay[back][front].decaydt = decaydt;

	      outval = deltaT;
	      implant[back][front].numcorr++;
	      decay[back][front].dnumcorr = implant[back][front].numcorr;
	      
	      /* Output decay time in 10 ms units on 10-bit spectrum
		 with 16-bit parameter declaration */
	      bdecay.corr.dtimecal = decay[back][front].dt*bdecayv.clock.calib;
	      bdecay.corr.dtime    = decay[back][front].time;
	      bdecay.corr.dde1     = (long)decay[back][front].dE1;
	      bdecay.corr.dde2     = (long)decay[back][front].dE2;
	      bdecay.corr.dde3     = (long)decay[back][front].dE3;
	      bdecay.corr.disum    = (long)decay[back][front].isum;
	      bdecay.corr.dimax    = (long)decay[back][front].imax;
	      bdecay.corr.dtof     = (long)decay[back][front].tof;
	      bdecay.corr.dnumcorr = (int)decay[back][front].dnumcorr;

	      //Set Decayed
	      implant[back][front].decayed = true;
	      	      
	    } else {
	      condition = 48; /* Correlation time too long */
	    }
	    /* Correlate first decay only */
	    // implant[back][front].implanted = false;  
	  } else {
	    condition = 52; /* Time between implants too short */
	  }
	} else {
	  condition = 36; /* Back to back decay */
	}
      } else {
	condition = 8; /* Invalid decay pixel */
      }
    } else {
      condition = 72; /* No implant found */
    }
    
  } else if (sega_only) {
    
    condition = 120;
    returnval += 8192;
    
    double dt = 1000000;
    long dE1 = 0;
    long dE2 = 0;
    long dE3 = 0;
    long isum = 0;
    long imax = 0;
    long tof = 0;
    
    double minimptime;
    double implant_time;
    implant_time = 0;
    
    /* Here we need to correlate gamma rays with implantations  */

    // find the most recent implant
    for (int i=lostripx; i<histripx; i++) {
      for (int j=lostripy; j<histripy; j++) {
  	if (implant[i][j].implanted) {
  	  /* Check any implants to see if they're within 
  	     40us of the implant. */
  	  if ( current_time != 0 ) {
  	    implant_time = (implant[i][j].time / 1000000.);
	    
  	    double deltaT;
  	    deltaT = current_time - implant_time;
	    
	    if (implant_time > current_time) {
  	      condition = 144; /* Bad isomer time */
  	    }
	    
  	    if ((deltaT) < (0.2)  &&  (deltaT >= 0.) ) { //200 us
  	      if ((deltaT) <  implant[i][j].dt) { //compare both on ms scale

  		/* Gamma is within (clockisomer.max)us following implant, 
  		   and dT is shorter than the last implant we found. */
  		dt   = deltaT*1000;
  		dE1  = implant[i][j].dE1;
  		dE2  = implant[i][j].dE2;
  		dE3  = implant[i][j].dE3;
  		isum = implant[i][j].isum;
  		imax = implant[i][j].imax;
  		tof  = implant[i][j].tof;
  	      }
  	    }
  	  }
  	}
      }
    }
    
    /* Fill "isomer-correlated" parameters */
    bdecay.corr.gtimecal = dt;
    bdecay.corr.gde1 = (long)dE1;
    bdecay.corr.gde2 = (long)dE2;
    bdecay.corr.gde3 = (long)dE3;
    bdecay.corr.gisum = (long)isum;
    bdecay.corr.gimax = (long)imax;
    bdecay.corr.gtof = (long)tof;
    
  } else {
    unknown_event = true;
    if (condition == 0) {
      condition = 20; /* Unknown event */
      returnval += 4096;
      
      if ((bdecayv.hit.fronthi != 0) && (bdecayv.hit.backhi == 0)) {
	condition = 24; /* DSSD front, no back */
      }
      if ((bdecayv.hit.fronthi == 0) && (bdecayv.hit.backhi != 0)) {
	condition = 28; /* DSSD back, no front */
      }
    }
  }
  
  /* End main correlator */

  /* Add one section for short lifetime look in planar detector */

  // cout<<"condition: "<<condition<<"  ";
  
  if(decay_event){
    //condition = 160;
    returnval += 8192;
    
    double dt = 1000000;
    long dE1 = 0;
    long dE2 = 0;
    long dE3 = 0;
    long isum = 0;
    long imax = 0;
    long tof = 0;
    double fronthiecal[17];
    double backhiecal[17];
    for(int z=0;z<17;z++) fronthiecal[z] = backhiecal[z] = 0;


    double minimptime;
    double implant_time;
    implant_time = 0;
    
    /* Here we need to correlate gamma rays with implantations  */
    for (int i=lostripx; i<histripx; i++) {
      for (int j=lostripy; j<histripy; j++) {
  	if (implant[i][j].implanted) {
  	  /* Check any implants to see if they're within 
  	     40us of the implant. */
  	  if ( current_time != 0 ) {
  	    implant_time = (implant[i][j].time / 1000000.);
	    
  	    double deltaT;
  	    deltaT = current_time - implant_time;
	    
  	    if (implant_time > current_time) {
  	      condition = 144; /* Bad isomer time */
  	    }
	    
  	    if ((deltaT) < (0.2) && (deltaT > 0) ) { //0.200 ms
	      dt = implant[i][j].dt;
	      if ((deltaT) <  dt) { //compare both on ms scale
  		/* Gamma is within (clockisomer.max)us following implant, 
  		   and dT is shorter than the last implant we found. */
  		dt   = deltaT*1000;
  		dE1  = implant[i][j].dE1;
  		dE2  = implant[i][j].dE2;
  		dE3  = implant[i][j].dE3;
  		isum = implant[i][j].isum;
  		imax = implant[i][j].imax;
  		tof  = implant[i][j].tof;
	        
  	      }
  	    }
  	  }
  	}
      }
    }
    
    /* Fill "isomer-correlated" parameters */
    bdecay.corr.gdtimecal = dt;
    bdecay.corr.gdde1 = (long)dE1;
    bdecay.corr.gdde2 = (long)dE2;
    bdecay.corr.gdde3 = (long)dE3;
    bdecay.corr.gdisum = (long)isum;
    bdecay.corr.gdimax = (long)imax;
    bdecay.corr.gdtof = (long)tof;
    for(int z=0; z<17; z++){
      bdecay.corr.gdfronthiecal[z] = fronthiecal[z];
      bdecay.corr.gdbackhiecal[z] = backhiecal[z];
    }
    
  }



  if (resetcheck != 0) {
    condition = resetcheck; /* Reset array flag */
  }
  
  //  cout<<condition<<endl;
  
  // for(int x=1; x<17; x++) {
  //   if(bdecay.sega.ecal[x]>0) {
  //     cout<<bdecay.sega.ecal[x]<<endl;
  //     // if(bdecay.sega.ecal[x]>2028 && bdecay.sega.ecal[x]<2038) {
  //     // 	string crap;
  //     // 	cin>>crap;
  //     // }
  //     // if(bdecay.sega.ecal[x]>1254 && bdecay.sega.ecal[x]<1264) {
  //     // 	string crap;
  //     // 	cin>>crap;
  //     // }

  //     if(condition == 32) {
  // 	string crap;
  // 	cin>>crap;
  //     }
  //   }
  // }
  
  bdecay.corr.flag = (long) condition;

  // if(condition<100) {
  //   cout<<condition<<"  "<<  bdecay.corr.flag <<endl;
  // }
  
  return outval;
  
}













