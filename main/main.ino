#include "../calc.h"

char* outputString;

void setup() {
    char input[] = "1+1";
    EXPRESSION_ELEMENT output;
    
    // --------------------------------------------------------

    calculate(input, &output);

    // --------------------------------------------------------

    outputString = IS_ERROR(output.flags)
        ? getErrorMessage(output.content.symbol_char)
        : getResultString(output);


	Serial.begin(9600); // ?

    if (IS_ERROR(output.flags))
    {
      Serial.println(outputString);
	  delay(3000);
	  Serial.println(input);
    }
	else {
		Serial.println(outputString);
	}

    // --------------------------------------------------------
}

void loop() {
	Serial.println(outputString);
}