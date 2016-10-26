SinOsc saw => Sporth sporth => dac;
sporth.parse("in dup dup 8 8 8000 zrev drop 0.8 * +");

saw.freq(300);
saw.amp(0.1);
1::second => now;
