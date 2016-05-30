#include "Configuration.h"

#include "mcp4728.h"

#ifdef DAC_I2C
bool dac_present = false;
const uint8_t dac_order[NUM_AXIS] = DAC_I2C_STEPPER_ORDER;

mcp4728 stepper_current_dac(DAC_I2C_STEPPER_ADDRESS);

int dac_init()
{
	int i;
	stepper_current_dac.begin();

	if(stepper_current_dac.reset() != 0)
		return -1;

	dac_present = true;

	for(i=0;i<NUM_AXIS;i++) {
		stepper_current_dac.setVref(i, DAC_I2C_STEPPER_VREF);
		stepper_current_dac.setGain(i, DAC_I2C_STEPPER_GAIN);
	}

	return 0;
}

void dac_current_percent(uint8_t channel, float val)
{
	if(!dac_present) return;

	if(val > 100)
		val = 100;

	stepper_current_dac.analogWrite(dac_order[channel],
			val*DAC_I2C_STEPPER_MAX/100);
	stepper_current_dac.update();
}

void dac_current_raw(uint8_t channel, uint16_t val)
{
	if(!dac_present) return;

	if(val > DAC_I2C_STEPPER_MAX)
		val = DAC_I2C_STEPPER_MAX;

	stepper_current_dac.analogWrite(dac_order[channel], val);
	stepper_current_dac.update();
}

void dac_print_values()
{
	if(!dac_present) return;

	SERIAL_ECHO_START;
	SERIAL_ECHOLNPGM("Stepper current values [%(raw)]:");
	SERIAL_ECHO_START;
	SERIAL_ECHOPAIR(" X:",
		100.0*stepper_current_dac.getValue(dac_order[0])/DAC_I2C_STEPPER_MAX);
	SERIAL_ECHOPAIR("(",
		(long unsigned int) stepper_current_dac.getValue(dac_order[0]));
	SERIAL_ECHOPAIR(") Y:",
		100.0*stepper_current_dac.getValue(dac_order[1])/DAC_I2C_STEPPER_MAX);
	SERIAL_ECHOPAIR("(",
		(long unsigned int) stepper_current_dac.getValue(dac_order[1]));
	SERIAL_ECHOPAIR(") Z:",
		100.0*stepper_current_dac.getValue(dac_order[2])/DAC_I2C_STEPPER_MAX);
	SERIAL_ECHOPAIR("(",
		(long unsigned int) stepper_current_dac.getValue(dac_order[2]));
	SERIAL_ECHOPAIR(") E:",
		100.0*stepper_current_dac.getValue(dac_order[3])/DAC_I2C_STEPPER_MAX);
	SERIAL_ECHOPAIR("(",
		(long unsigned int) stepper_current_dac.getValue(dac_order[3]));	
	SERIAL_ECHOLN(")");
}

void dac_commit_eeprom()
{
	if(!dac_present) return;

	stepper_current_dac.eepromWrite();
}
#endif
