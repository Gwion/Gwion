Sporth s => dac;

//"0 p 0.01 port mtof 1 p 0.003 port 1 1 2 p 0.01 port fm 0.5 * dup dup 0.94 10000 revsc drop 0.1 * +" => 
//	string str;
//"0.5 * dup dup 0.94 10000 revsc drop 0.1 * +" +=> str;
//"0 p 0.01 port mtof 1 p 0.003"
// => string str;

//s.parse("0 p 0.01 port mtof 1 p 0.003 port 1 1 2 p 0.01 port fm 0.5 * dup dup 0.94 10000 revsc drop 0.1 * +");
//<<<str>>>;
//str => s.parse;

s.parse("0 p 0.01 port mtof 1 p 0.003 port 1 1 2 p 0.01 port fm 0.5 * dup dup 0.94 10000 revsc drop 0.1 * +");
110 => float bpm;

(60.0 / bpm) :: second => dur t;

//[0, 3, 7, 10] @=> int scale[];
int scale[4];
0  => scale[0];
3  => scale[1];
7  => scale[2];
10 => scale[3];

0 => int stp;
s.p(1, 0.5);
0 => int nbars;
60 => int base;

while(1)
{
    if(stp == 0) {
        s.p(1, 0.7);
        nbars++;
    } else s.p(1, 0.4);
    if(nbars == 5) {
        if(base == 60) 53 => base; else 60 => base;
        1 => nbars;
    }
    s.p(0, base + scale[stp] + 12 * Math.rand2(-1, 1));
    s.p(2, Math.rand2f(0.1, 3));
    t * 0.25 => now;
//    (stp + 1) % scale.size() => stp;
	(stp + 1) % 4 => stp;
}
