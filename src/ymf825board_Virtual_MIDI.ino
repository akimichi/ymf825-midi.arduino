#include <MIDI.h> //MIDIライブラリの読み込みを指定

extern "C" {
#include "fmif.h"
}
#include "fmsd1.h"

/* usb-midi */
#include <usbh_midi.h>
#include <usbhub.h>
USB Usb;
USBHub Hub(&Usb);
USBH_MIDI  Midi(&Usb);

void MIDI_poll();

void onInit()
{
  char buf[20];
  uint16_t vid = Midi.idVendor();
  uint16_t pid = Midi.idProduct();
  sprintf(buf, "VID:%04X, PID:%04X", vid, pid);
  Serial.println(buf); 
}
/* end of usb-midi */


MIDI_CREATE_DEFAULT_INSTANCE(); //MIDIクラスからMIDIインスタンスを生成

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  Fmdriver_sendMidi(0x90);
  Fmdriver_sendMidi(pitch);
  Fmdriver_sendMidi(velocity);
}
void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  Fmdriver_sendMidi(0x80);
  Fmdriver_sendMidi(pitch);
  Fmdriver_sendMidi(velocity);
}
void handleProgramChange(byte channel, byte number)
{
  Fmdriver_sendMidi(0xc0);
  Fmdriver_sendMidi(number);
}
void setup() {
  /* usb-midi */
  Serial.begin(115200);


  // put your setup code here, to run once:
  initSPI();
  initSD1();

  Fmdriver_init();
  /* MIDI.setHandleNoteOn(handleNoteOn); */
  /* MIDI.setHandleNoteOff(handleNoteOff); */
  /* MIDI.setHandleProgramChange(handleProgramChange); */
  /* MIDI.begin(MIDI_CHANNEL_OMNI); //全てのチャンネルのメッセージを受信 */
  /* Serial.begin(38400); //HairlessMIDIで設定した転送速度と合わせる。 */

  if (Usb.Init() == -1) {
    while (1); //halt
  }//if (Usb.Init() == -1...
  delay( 200 );

  // Register onInit() function
  Midi.attachOnInit(onInit);
  /* end of usb-midi */

}

void loop() {
  // put your main code here, to run repeatedly:
  /* MIDI.read(); */
  /* usb-midi */
  Usb.Task();
  if ( Midi ) {
    MIDI_poll();
  }
  /* end of usb-midi */
}

// Poll USB MIDI Controler and send to serial MIDI
void MIDI_poll()
{
  char buf[16];
  uint8_t bufMidi[MIDI_EVENT_PACKET_SIZE];
  uint16_t  rcvd;

  if (Midi.RecvData( &rcvd,  bufMidi) == 0 ) {
    uint32_t time = (uint32_t)millis();
    sprintf(buf, "%04X%04X:%3d:", (uint16_t)(time >> 16), (uint16_t)(time & 0xFFFF), rcvd); // Split variable to prevent warnings on the ESP8266 platform
    Serial.print(buf);

    for (int i = 0; i < MIDI_EVENT_PACKET_SIZE; i++) {
      sprintf(buf, " %02X", bufMidi[i]);
      Serial.print(buf);
    }
    Serial.println("");
  }
}
