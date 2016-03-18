 
from __future__ import print_function

from os import environ, path

from pocketsphinx.pocketsphinx import *
from sphinxbase.sphinxbase import *

import pyaudio
import sys


MODELDIR = "data/speech/model"
CHESSGRAMMARDIR="data/speech/wcgrammar"

class WCSpeech:


    def __init__(self):
        
        # Create a decoder with certain model
        config = Decoder.default_config()
        config.set_string('-hmm', path.join(MODELDIR, 'en-us'))
        config.set_string('-fdict', path.join(MODELDIR, 'en-us/noisedict'))
 
        config.set_string('-dict', path.join(CHESSGRAMMARDIR, 'wc.dict'))
        config.set_string('-jsgf', path.join(CHESSGRAMMARDIR, 'wc.jsgf'))

        config.set_string('-mllr', path.join(MODELDIR, 'mllr_matrix'))
        config.set_string('-logfn', '/dev/null')	#comment this file to get verbose output
        config.set_string('-backtrace', 'yes')
        config.set_float('-fillprob', 20)
        
        self.decoder = Decoder(config)
        self.p = pyaudio.PyAudio()

    def recognize(self):
        stream = self.p.open(format=pyaudio.paInt16, channels=1, rate=16000, input=True, frames_per_buffer=1024)
        stream.start_stream()
        in_speech_bf = True
        self.decoder.start_utt()
        oldlen=0
        i=0
        while True:
            buf = stream.read(1024)
            if buf:
                self.decoder.process_raw(buf, False, False)
                try:
                    if  self.decoder.hyp().hypstr != '':
                        #clear line
                        print(' '*oldlen,end='\r')
                        oldlen=17+len(self.decoder.hyp().hypstr)
                        print('You were saying?', self.decoder.hyp().hypstr, end='')
                        i=0
                        pass
                except AttributeError:
                    pass
                if self.decoder.get_in_speech():
                    #speech input dot effect @TODO perfect or remove this
                    i=i+1
                    sys.stdout.write('.')
                    if i%5==0:
                        sys.stdout.write('\b'*5)
                        i=0
                    sys.stdout.flush()
                if self.decoder.get_in_speech() != in_speech_bf:
                    in_speech_bf = self.decoder.get_in_speech()
                    if not in_speech_bf:
                        self.decoder.end_utt()
                        try:
                            if  self.decoder.hyp().hypstr != '':
                                print('\r\nStream decoding result:', self.decoder.hyp().hypstr)
                                return self.decoder.hyp().hypstr
                        except AttributeError:
                            pass
                        self.decoder.start_utt()
            else:
                break
        self.decoder.end_utt()
        print('An Error occured:', self.decoder.hyp().hypstr)




if __name__ == "__main__":
	s=SpeechRec()
	while True:
		s.recognize()

