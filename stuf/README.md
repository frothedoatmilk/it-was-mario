## STUF - Synthetic Tone Unicode Format

### Protocol 1

#### Formatting fundamentals

There are four different modes marked by XML markers in 
protocol 1 - `chord`, `templ`, `voice`, and `param`. The 
`param` marker allows for simple parameters to be input 
about the song, such as the tempo. The `voice` marker 
allows for an input of a collection of waveforms for 
each of the different voices. These are always 
capitalized and are either SINE, SAWTOOTH, TRIANGLE, or 
SQUARE. The last two markers take a little more 
explaining. Both allow for the actual description of 
music as notes within chords. The difference between 
thw two is that in `templ`, musical 'templates' can be 
formed, such as verses and choruses that can be 
repeated through their respective markers in `chord`. 
The template names in `templ` are started with a `>` 
symbol followed by the symbol name, and is ended by 
a `<` symbol, typically followed by the symbol name 
again although this is not required. Within the scope 
of `chord`, these templates can be called by the same 
initial marker in the templates.  
_Note_: Whitespace is important to separate chords, 
but the amount of whitespace, including empty lines, 
are ignored during interpretting.

#### Chord structure

Within the chord based scopes, notes are written 
starting with a letter and possibly other symbols 
that describe the length of the note. These letters 
are w, h, q, T, e, t, s, and x in the current version 
for whole, half, quarter, quarter-note triplet, 
eighth, eighth-note triplet, sixteenth node, and 
sixteenth-note triplet respectively. After this can 
also be a dot, extending the length as is common in 
music notation. 

After this lenghth description, a space is required 
to separate each of the notes to be played at this 
demarkation of time. Each of these notes is separated 
by a space, and the type of note played is determined 
by their order and the input from the `voice` marker. 
There cannot be more notes listed than voices 
specified above for this reason. What happens in this 
case is probably some generic C++ error, though this 
hasn't been tested as of writing this specification. 

Notes are described using a capital letter followed 
by a possible sharp (#) or flat (b) symbol, if an 
accidental needs to be applied. Then a number 
describing the octave is used. This number is _not_ 
the standard one to describe notes on a piano. This 
system is similar in that the lowest A on a piano 
is A0. However, that entire lowest octave is also 
in the zeroth octave unlike the standard. Each A 
is the lowest representative of its octave, with 
the higest being G. After this, a number of extra 
tags can be used. The tags `!`, `:`, `;`, and `,` 
will add some space between successive notes which 
is useful when two notes are to be played back to 
back but they are distinct notes. A length tag 
can also be added which specifies that the note 
length is different than the length of time for 
the sound epoch of this row. For example, although 
only a quarter note is being described for the 
current row, the now A4w would specify that a 
whole note should be played beyond the end of this 
sound epoch. It's a little confusing, I know. 
Take a look at a few STUF examples and it 
hopefully should clear things up. It also should 
be known that there isn't specifically an order 
to these characters. As long as they occur in 
some order before a space, the behavior should 
be identical. 

Lastly, note descriptions can be replaced by a `-`. 
These symbols are merely to allow for a voice to be
skipped in the current sound epoch so other, 
higher index voices can be used. That same symbol 
is also used for comments in the code, which is 
very useful in marking where measures begin 
and end for us humans (as I am certainly an 
organic being).
