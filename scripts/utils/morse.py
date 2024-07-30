# Python program to implement Morse Code Translator
'''
VARIABLE KEY
'cipher' -> 'stores the morse translated form of the english string'
'decipher' -> 'stores the english translated form of the morse string'
'citext' -> 'stores morse code of a single character'
'i' -> 'keeps count of the spaces between morse characters'
'message' -> 'stores the string to be encoded or decoded'
'''
import sys

# Dictionary representing the morse code chart
MORSE_CODE_DICT = {
	'A':'.-', 'B':'-...', 'C':'-.-.', 'D':'-..', 'E':'.', 'F':'..-.',
	'G':'--.', 'H':'....', 'I':'..', 'J':'.---', 'K':'-.-', 'L':'.-..',
	'M':'--', 'N':'-.', 'O':'---', 'P':'.--.', 'Q':'--.-', 'R':'.-.',
	'S':'...', 'T':'-', 'U':'..-', 'V':'...-', 'W':'.--', 'X':'-..-',
	'Y':'-.--', 'Z':'--..', '1':'.----', '2':'..---', '3':'...--',
	'4':'....-', '5':'.....', '6':'-....', '7':'--...', '8':'---..',
	'9':'----.', '0':'-----', ', ':'--..--', '.':'.-.-.-', '?':'..--..',
	'/':'-..-.', '-':'-....-', '(':'-.--.', ')':'-.--.-'
}

# Function to encrypt the string
# according to the morse code chart
def createCodes(hdrFile, text):
	tag = text.replace(" ", "_").lower()
	hdrFile.write(f'/* morse beeper code array for "{text}" */\n')
	hdrFile.write(f'static const struct buzzer_alert_step _{tag}_morse_beep_steps[] = {{\n')
	for letter in text:
		if letter != ' ':
			# Looks up the dictionary and adds the
			# corresponding morse code
			# along with a space to separate
			# morse codes for different characters
			code = MORSE_CODE_DICT[letter]
			hdrFile.write(f'/* morse encoded "{letter}" Char */\n')
			for idx in range(0, len(code)):
				if code[idx] == '-':
					hdrFile.write('\t{.freq = MORSE_DAA_FREQ, .ms = MORSE_TIME(3)},\n')
				else:
					hdrFile.write('\t{.freq = MORSE_DIT_FREQ, .ms = MORSE_TIME(1)},\n')
				hdrFile.write('\t{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(1)},\n')
			hdrFile.write('\t{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(3)},\n')
		else:
			# 1 space indicates different characters
			# and 2 indicates different words
			hdrFile.write(f'/* morse encoded word space */\n')
			hdrFile.write('\t{.freq = MORSE_SPACE_FREQ, .ms = MORSE_TIME(7)},\n')

	hdrFile.write('};\n')
	hdrFile.write(f'\n')
	hdrFile.write(f'static const struct user_alerts_pattern _{tag}_morse_beeps = {{\n')
	hdrFile.write(f'	.steps = _{tag}_morse_beep_steps,\n')
	hdrFile.write(f'	.steps_count = ARRAY_SIZE(_{tag}_morse_beep_steps),\n')
	hdrFile.write(f'	.loop_count = 1,\n')
	hdrFile.write(f'}};\n')

# Hard-coded driver function to run the program
def main():
	args = sys.argv[1:]
	if len(args) == 0:
		print('argument input is missing')
	else:
		hdrFilePath = f"morse_beep_codes.h"
		with open(hdrFilePath, 'w') as hdrFile:
			hdrFile.write('#include <zephyr/sys/util.h>\n')
			hdrFile.write('#include <user_alerts/user_alerts.h>\n')
			hdrFile.write('\n')
			hdrFile.write('#define MORSE_DAA_FREQ	4400\n')
			hdrFile.write('#define MORSE_DIT_FREQ	3600\n')
			hdrFile.write('#define MORSE_SPACE_FREQ	0\n')
			hdrFile.write('#define MORSE_TIME(S)	((S) * 100)\n')
			hdrFile.write('\n')
			for idx in range(0, len(args)):
				createCodes(hdrFile, args[idx].upper())

# Executes the main function
if __name__ == '__main__':
	main()
