#include "Files.h"
#include "MicroBit.h"
#include "MicroBitUARTService.h"
#include "Set.h"
#include "TwoDigitDisplay.h"
#include "repcount.h"
#define INCREMENT_A 10
#define INCREMENT_B 1
#define READSERIAL
//#define TESTREMOVE
//#define BLETEST
MicroBit uBit;
MicroBitUARTService *uart;

TwoDigitDisplay twodigit;
ManagedString filename;
Set meinsatz;
ManagedString delimiter("\n");
ManagedString setting;
int inputBuff = 0;
int inputterm = 0;
int terminator = 0;
int connected = 0;

void onConnected(MicroBitEvent) {
  uBit.display.scroll("C");

  connected = 1;
}

void onDisconnected(MicroBitEvent) {
  uBit.display.scroll("D");
  connected = 0;
}
void term_input(MicroBitEvent) {
	inputterm = 1;
}
void printfile(MicroBitEvent e) { readToSerial(ManagedString(filename)); }
void sendfile(MicroBitEvent e) { readtoBLE(filename); }

int int_from_serial() {
  return atoi(uBit.serial.readUntil(delimiter).toCharArray());
}

ManagedString string_from_serial() { return uBit.serial.readUntil(delimiter,SYNC_SPINWAIT); }
void inputButton(MicroBitEvent e) {
  inputBuff = 0;
  if (e.source == MICROBIT_ID_BUTTON_A) {
    if (e.value == MICROBIT_BUTTON_EVT_CLICK)
      inputBuff = inputBuff + INCREMENT_A;
		if (e.value == MICROBIT_BUTTON_EVT_HOLD) {
			inputBuff = inputBuff - INCREMENT_A;
			uBit.sleep(300);
			while (uBit.buttonA.isPressed()) {

				uBit.sleep(150);
      inputBuff = inputBuff - INCREMENT_A;
  }
		}
	}
  if (e.source == MICROBIT_ID_BUTTON_B) {
    if (e.value == MICROBIT_BUTTON_EVT_CLICK)
      inputBuff = inputBuff + INCREMENT_B;
		if (e.value == MICROBIT_BUTTON_EVT_HOLD) {

			inputBuff = inputBuff - INCREMENT_B;
			uBit.sleep(300);
			while (uBit.buttonB.isPressed()) {
				uBit.sleep(150);
      inputBuff = inputBuff - INCREMENT_B;
  }
}

ManagedString string_from_ble() { return uart->readUntil(delimiter); }

int input() {
  int ret = 0;

	while (true) {
    ret = ret + inputBuff;
    inputBuff = 0;
    // uBit.display.printAsync(ret);
    uBit.display.printAsync(twodigit.createImage(ret));
		uBit.sleep(INPUTLAG);
    //    uBit.display.clear();
		if (inputterm == 1) {
			inputterm = 0;
			return ret;
  }
	}


}
int input(int previous_val) {
	int ret = previous_val;
	int i = 0;
	while (true) {
		if (inputBuff == 0 and i != 0) {
			/*if (goback != 0) {
				return ret;
			}*/
			if (inputterm == 1) {
				inputterm = 0;
  return ret;
}

			if (i > DISPLAYTIME) {
				uBit.display.clear();
			}
		}
		else {
			i = 0;
    ret = ret + inputBuff;
			ret = abs(ret);
    inputBuff = 0;
    uBit.display.printAsync(twodigit.createImage(ret));
  }
		uBit.sleep(INPUTLAG);
		i++;
}
}
int input(const char *message) {
  int ret = 0;
	uBit.display.scroll(message);
  while (0 == uBit.buttonA.isPressed() and 0 == uBit.buttonB.isPressed()) {
		uBit.sleep(INPUTLAG);
  }

	int i = 0;
	while ((true)) {
		if (inputBuff == 0 and i != 0) {
			if (goback != 0) {
				return ret;
			}
			if (inputterm == 1) {
				inputterm = 0;
				return ret;
			}

			if (i > DISPLAYTIME) {
				uBit.display.clear();
			}
		}
		else {
			i = 0;
    ret = ret + inputBuff;
			ret = abs(ret);
    inputBuff = 0;
    uBit.display.printAsync(twodigit.createImage(ret));
  }
		uBit.sleep(INPUTLAG);
		i++;
}
}


int input(char message) {
	int ret = 0;
	uBit.display.printChar(message);
	while (0 == uBit.buttonA.isPressed() and 0 == uBit.buttonB.isPressed()) {
		uBit.sleep(INPUTLAG);
	}

	while (true) {
		ret = ret + inputBuff;
		inputBuff = 0;
		// uBit.display.printAsync(ret);
		uBit.display.printAsync(twodigit.createImage(ret));
		uBit.sleep(INPUTLAG);
		//    uBit.display.clear();
		if (inputterm == 1) {
			inputterm = 0;
			return ret;
	}
	}
}
///*time in 1/10 sekunden
int input(int time, int previous_val) {
	int ret = previous_val;
	int i = 0;
	while ((i < time)) {
		if (inputBuff == 0 and i != 0) {
			if (goback != 0) {
				return ret;
			}
			if (inputterm == 1) {
				inputterm = 0;
				return ret;
			}

			if (i > DISPLAYTIME) {
	uBit.display.clear();
			}
}
		else {
			i = 0;
		ret = ret + inputBuff;
			ret = abs(ret);
			// uBit.display.printAsync(ret);
		inputBuff = 0;
		uBit.display.printAsync(twodigit.createImage(ret));
		}
		uBit.sleep(INPUTLAG);
		//if (goback != 0) {
		//	return ret;
		//}
		//if (inputterm == 1) {
		//	inputterm = 0;
		//	return ret;
		//}
		i++;
	}
		//    uBit.display.clear();

	uBit.io.P1.setDigitalValue(0);
	uBit.io.P8.setDigitalValue(0);

	return ret;
}
void serial_input(MicroBitEvent e) {
  ManagedString buffer;
  buffer = string_from_serial();
  uBit.serial.printf("input: %s", buffer.toCharArray());
  // meinsatz.set_name(buffer);
}
void setloop() {


	int loop_case = 0;
	int oldname;
	while (true) {
		// prüfe ob Set Loop verlassen werden soll.
		loop_case = loop_case % 3;
		switch (loop_case)
		{
		case 0:
		uBit.display.scroll("ID");
			uBit.display.clear();
			oldname = meinsatz.get_ID();

			meinsatz.set_ID(input(WAITFORINPUT, meinsatz.get_ID()));
		uBit.sleep(500);
			if (oldname != meinsatz.get_ID()) {

		meinsatz.set_name(read_to_string(ManagedString(meinsatz.get_ID())));
				uBit.display.scroll(meinsatz.get_name(), LONGTEXTDELAY);
			}
			if (meinsatz.get_name() == ManagedString("")) {
				meinsatz.set_name(ManagedString("-"));
			}
		uBit.sleep(500);
			loop_case++;
			break;
		case 1:

			uBit.display.scroll("Kg", LONGTEXTDELAY);

			if (goback != 0) {
				break;
			}
			meinsatz.set_weight(input(WAITFORINPUT, meinsatz.get_weight()));

		uBit.sleep(500);
			loop_case += 1 - goback;
			break;

		case 2:
			uBit.display.scroll("WDH", LONGTEXTDELAY);
			if (goback != 0) {
				break;
			}
			meinsatz.set_reps(repcount(inputBuff, inputterm, goback, pause_setting));
		uBit.sleep(500);
		uBit.display.clear();
			loop_case += 1 - goback;
			break;
		default:

			break;
		}

		goback = 0;

		if (terminator == 1) {
			terminator = 0;
			break;
		}
	}
		meinsatz.write_to_file(filename);  // schreibe den Satz ins file
		uBit.sleep(500);
	return;
}
		
void catalog_read(MicroBitEvent) {
	ManagedString exercisenumber;
	ManagedString exercisename;
	uBit.serial.printf("starte\n");
	uBit.display.scroll("usb input");
	do {
		exercisenumber = string_from_serial();
		uBit.sleep(5);
		exercisename = string_from_serial();
		uBit.sleep(5);
		if (!remove_file(exercisenumber)) {
			uBit.serial.printf("failed to remove file");

	}


		uBit.sleep(5);

		appendLine(exercisenumber, exercisename);

		uBit.sleep(5);



	} while (!(exercisenumber == ManagedString("ENDE")));
	if (!remove_file(ManagedString("ENDE"))) {
		uBit.serial.printf("failed to remove file");
		uBit.serial.send(exercisenumber);
}
	uBit.sleep(5);
	uBit.serial.printf("microbit ende");
	uBit.display.scrollAsync("mitte");
	uBit.serial.printf("begin loop\n");
	uBit.display.scroll("check 1:");
	uBit.display.scroll(read_to_string(ManagedString("1")));
	for (int i = 1; i <= 100; i++)
	{
		const int x = i;
		uBit.serial.printf("%d: ", i);
		readToSerial(ManagedString(x));
		// uBit.display.scroll(read_to_string(x));
	}
	uBit.display.scrollAsync("ende");


}
void onConnected(MicroBitEvent) {
	uBit.display.scroll("C");

	connected = 1;
}

void onDisconnected(MicroBitEvent) {
	uBit.display.scroll("D");
	connected = 0;
}
void term_input(MicroBitEvent) {
	uBit.display.printCharAsync('>', 200);
	inputterm = 1;
}
void terminate(MicroBitEvent e) {
	terminator = 1;
	uBit.display.printCharAsync('*', 200);
}
void printfile_toserial(MicroBitEvent e) { readToSerial(ManagedString(filename)); }
void ble_sendfile(MicroBitEvent e) { readtoBLE(filename); }
void go_back(MicroBitEvent e) {
	goback = 2;
	uBit.display.printCharAsync('<', 200);
}

}
int main() {
  // Initialise the micro:bit runtime.
  // delimiter = "\n";
  // TODO:
  /*
  automatisches zählen der reps
  (Beschleunigungswerte speichern/senden)
  Key Values speichern für Trainingsmittelkatalog
  */
  uBit.init();
  uBit.serial.baud(9600);
  uBit.serial.send("alive");
  initrepcount();
  filename = "log.csv";  // file to store the training log
  uint8_t *stor;  // storing the setting if microbit has started , Warning
                  // uninitialized
  *stor = 1;      // dereferenced ptr. inhalt ist 1
  setting = "fileexist";  // name des settings,storage key value pair
  

  uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED,
                         onConnected);  // entscheide ob eingabe über ble
  uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED,
                         onDisconnected);
  uart = new MicroBitUARTService(*uBit.ble, 32, 32);  // ble service
                       // info

  uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_EVT_ANY,
                         inputButton);
  uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_EVT_ANY,
                         inputButton);
  uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_LONG_CLICK,
                         terminate);
  uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK,
	  term_input);
  //uBit.messageBus.listen(MICROBIT_ID_SERIAL, MICROBIT_SERIAL_EVT_DELIM_MATCH,
    //                     serial_input);
  if (NULL == uBit.storage.get(setting)) {
    uBit.storage.put(setting, stor, sizeof(stor));
    appendLine(filename, ManagedString("date,name,id,weight,reps"));
  }  // prueft ob der microbit zum ersten mal in das file schreibt.
  // dann wird die header dafür geschrieben. (csv format header)
  uBit.display.scroll("ble");
   
  // warten auf Verbindung mit Smartphone
  // uBit.serial.send(
  //     ManagedString("gebe die ID, das Gewicht, die Reps, ein! via "
  //                   "buttonsaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
  // uBit.serial.send("datum:");

  // wenn Verbunden lese das Datum ein (von Handy App)
#ifdef BLETEST
  if (connected == 1)
  {
	  uart->send("gebe Name ein");
	  ManagedString buffer = string_from_ble();
	  uBit.storage.put("1", (uint8_t*)buffer.toCharArray(), sizeof(buffer.toCharArray()));

	  KeyValuePair* testt = uBit.storage.get("1");
	  const char * stringname = (const char*)testt->value;
	  uBit.display.scroll(stringname);

  }
  //compiles!

  if (connected == 1) {
	  uart->send("gebe Name ein");
	  ManagedString exercisename = string_from_ble();
	  uBit.sleep(300);
	  uart->send("gebe Nr. ein");
	  ManagedString exercisenumber = string_from_ble();
	  uBit.sleep(300);
	  remove_file(exercisenumber);

	  uBit.sleep(300);

	  appendLine(exercisenumber, exercisename);
	  uBit.sleep(300);
	  readtoBLE(exercisenumber);
	  uBit.sleep(300);
	  uBit.display.scroll(read_to_string(exercisenumber));
  }

  if (connected == 1) {
	  uart->send(ManagedString("Datum: 3x T,M,J newline"));
	  meinsatz.set_date(atoi(string_from_ble().toCharArray()),
		  atoi(string_from_ble().toCharArray()),
		  atoi(string_from_ble().toCharArray()));
	  uart->send(ManagedString("ID, gewicht, reps"));
  }

  else {
	  uBit.display.scroll("Datum:");
	  meinsatz.set_date(input('T'), input('M'), input('J'));
  }
#endif // BLETEST


#ifdef READSERIAL //hella buggy
  ManagedString exercisenumber;
  ManagedString exercisename;
  uBit.serial.printf("starte\n");
  do {
	  exercisenumber = string_from_serial();
	  uBit.sleep(5);
	  exercisename = string_from_serial();
	  uBit.sleep(5);
	  if (!remove_file(exercisenumber)) {
		  uBit.serial.printf("failed to remove file") ;

	  }

	  
	uBit.sleep(5);

	  appendLine(exercisenumber, exercisename);

	  uBit.sleep(5);


	
  } while (!(exercisenumber == ManagedString("ENDE")));
  if (!remove_file(ManagedString("ENDE"))) {
	  uBit.serial.printf("failed to remove file");
	  uBit.serial.send(exercisenumber);
  }
  uBit.sleep(5);
  uBit.serial.printf("microbit ende");
  
  uBit.serial.printf("begin loop\n");
  for (int i = 1; i <= 100; i++)
  {
	  const int x = i;
	  uBit.serial.printf("%d: ", i);
	  readToSerial(ManagedString(x));
	 // uBit.display.scroll(read_to_string(x));
  }
  
	 

#endif // READSERIAL

#ifdef TESTREMOVE
  ManagedString fn("test");
  appendLine(fn, ManagedString("das ist ein Test"));
  readToSerial(fn);
  remove_file(fn);
  uBit.serial.send("removed");
  readToSerial(fn);
  uBit.serial.send("und?");
#endif // TESTREMOVE


  setloop();
  // ansonsten über Hand eingabe
  
  // uBit.serial.printf("gebe datum ein");

  // meinsatz.set_date(int_from_serial(), int_from_serial(),
  // int_from_serial());

  // automatischer / manueller Input der Sets
  
  uBit.messageBus.ignore(MICROBIT_ID_BUTTON_A, MICROBIT_EVT_ANY, inputButton);
  uBit.messageBus.ignore(MICROBIT_ID_BUTTON_B, MICROBIT_EVT_ANY, inputButton);
  uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK,
                         printfile);  // gebe file via serial aus
  uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK,
                         sendfile);  // gebe via BLE aus
  readtoBLE(filename);               // gebe via BLE aus
  uBit.serial.send("hello");

  uBit.sleep(10000);
  // Insert your code here!

  // If main exits, there may still be other fibers running or registered
  // event handlers etc. Simply release this fiber, which will mean we enter
  // the scheduler. Worse case, we then sit in the idle task forever, in a
  // power efficient sleep.
  release_fiber();
}
