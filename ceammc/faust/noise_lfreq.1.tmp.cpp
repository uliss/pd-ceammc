//----------------------------------------------------------
//
// Code generated with Faust 0.9.92 (http://faust.grame.fr)
//----------------------------------------------------------

/* link with  */
#include <math.h>
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif  


#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

class mydsp : public dsp {
  private:
	int 	iVec0[2];
	float 	fConst0;
	float 	fConst1;
	FAUSTFLOAT 	fslider0;
	float 	fRec1[2];
	float 	fConst2;
	float 	fRec7[2];
	float 	fRec8[2];
	int 	iRec9[2];
	float 	fRec6[2];
	float 	fRec5[2];
	float 	fRec4[2];
	float 	fRec3[2];
	float 	fRec2[2];
	float 	fRec0[2];
	int fSamplingFreq;

  public:
	virtual void metadata(Meta* m) { 
		m->declare("noise.lib/name", "Faust Noise Generator Library");
		m->declare("noise.lib/version", "0.0");
		m->declare("signal.lib/name", "Faust Signal Routing Library");
		m->declare("signal.lib/version", "0.0");
		m->declare("basic.lib/name", "Faust Basic Element Library");
		m->declare("basic.lib/version", "0.0");
		m->declare("miscoscillator.lib/name", "Faust Oscillator Library");
		m->declare("miscoscillator.lib/version", "0.0");
		m->declare("filter.lib/name", "Faust Filter Library");
		m->declare("filter.lib/version", "2.0");
		m->declare("math.lib/name", "Faust Math Library");
		m->declare("math.lib/version", "2.0");
		m->declare("math.lib/author", "GRAME");
		m->declare("math.lib/copyright", "GRAME");
		m->declare("math.lib/license", "LGPL with exception");
	}

	virtual int getNumInputs() { return 0; }
	virtual int getNumOutputs() { return 1; }
	static void classInit(int samplingFreq) {
	}
	virtual void instanceConstants(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fConst0 = min(1.92e+05f, max(1.0f, (float)fSamplingFreq));
		fConst1 = (3.1415927f / fConst0);
		fConst2 = (6.2831855f / fConst0);
	}
	virtual void instanceResetUserInterface() {
		fslider0 = 1e+04f;
	}
	virtual void instanceClear() {
		for (int i=0; i<2; i++) iVec0[i] = 0;
		for (int i=0; i<2; i++) fRec1[i] = 0;
		for (int i=0; i<2; i++) fRec7[i] = 0;
		for (int i=0; i<2; i++) fRec8[i] = 0;
		for (int i=0; i<2; i++) iRec9[i] = 0;
		for (int i=0; i<2; i++) fRec6[i] = 0;
		for (int i=0; i<2; i++) fRec5[i] = 0;
		for (int i=0; i<2; i++) fRec4[i] = 0;
		for (int i=0; i<2; i++) fRec3[i] = 0;
		for (int i=0; i<2; i++) fRec2[i] = 0;
		for (int i=0; i<2; i++) fRec0[i] = 0;
	}
	virtual void init(int samplingFreq) {
		classInit(samplingFreq);
		instanceInit(samplingFreq);
	}
	virtual void instanceInit(int samplingFreq) {
		instanceConstants(samplingFreq);
		instanceResetUserInterface();
		instanceClear();
	}
	virtual mydsp* clone() {
		return new mydsp();
	}
	virtual int getSampleRate() {
		return fSamplingFreq;
	}
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("0x00");
		ui_interface->addHorizontalSlider("freq", &fslider0, 1e+04f, 2e+01f, 2e+04f, 0.1f);
		ui_interface->closeBox();
	}
	virtual void compute (int count, FAUSTFLOAT** input, FAUSTFLOAT** output) {
		float 	fSlow0 = (0.001f * float(fslider0));
		FAUSTFLOAT* output0 = output[0];
		for (int i=0; i<count; i++) {
			iVec0[0] = 1;
			fRec1[0] = (fSlow0 + (0.999f * fRec1[1]));
			float fTemp0 = (1.0f / tanf((fConst1 * fRec1[0])));
			float fTemp1 = (fTemp0 + 1);
			float fTemp2 = (0 - ((1 - fTemp0) / fTemp1));
			float fTemp3 = (fConst2 * fRec1[0]);
			float fTemp4 = sinf(fTemp3);
			float fTemp5 = cosf(fTemp3);
			fRec7[0] = ((fRec8[1] * fTemp4) + (fRec7[1] * fTemp5));
			fRec8[0] = (((fRec8[1] * fTemp5) + (fRec7[1] * (0 - fTemp4))) + (1 - iVec0[1]));
			int iTemp6 = ((fRec7[1] <= 0) & (fRec7[0] > 0));
			iRec9[0] = ((1103515245 * iRec9[1]) + 12345);
			fRec6[0] = ((fRec6[1] * (1 - iTemp6)) + (4.656613e-10f * (iTemp6 * iRec9[0])));
			fRec5[0] = ((fTemp2 * fRec5[1]) + ((fRec6[1] + fRec6[0]) / fTemp1));
			fRec4[0] = ((fTemp2 * fRec4[1]) + ((fRec5[1] + fRec5[0]) / fTemp1));
			fRec3[0] = ((fTemp2 * fRec3[1]) + ((fRec4[1] + fRec4[0]) / fTemp1));
			fRec2[0] = ((fTemp2 * fRec2[1]) + ((fRec3[1] + fRec3[0]) / fTemp1));
			fRec0[0] = ((fRec0[1] * fTemp2) + ((fRec2[1] + fRec2[0]) / fTemp1));
			output0[i] = (FAUSTFLOAT)fRec0[0];
			// post processing
			fRec0[1] = fRec0[0];
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fRec6[1] = fRec6[0];
			iRec9[1] = iRec9[0];
			fRec8[1] = fRec8[0];
			fRec7[1] = fRec7[0];
			fRec1[1] = fRec1[0];
			iVec0[1] = iVec0[0];
		}
	}
};


