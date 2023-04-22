#include <MIDI.h> //MIDIライブラリの読み込みを指定

extern "C" {
#include "fmif.h"
}
#include "fmsd1.h"

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
  // put your setup code here, to run once:
  initSPI();
  initSD1();

  Fmdriver_init();
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleProgramChange(handleProgramChange);
  MIDI.begin(MIDI_CHANNEL_OMNI); //全てのチャンネルのメッセージを受信
  Serial.begin(38400); //HairlessMIDIで設定した転送速度と合わせる。
}

void loop() {
  // put your main code here, to run repeatedly:
  MIDI.read();
}
