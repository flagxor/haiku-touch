#include "steno.h"
#include "captouch.h"

enum Buttons {
  NUM=(1L<<0),
  iS=(1L<<1), iT=(1L<<2), iK=(1L<<3), iP=(1L<<4), iW=(1L<<5), iH=(1L<<6), iR=(1L<<7),
  A=(1L<<8), O=(1L<<9), STAR=(1L<<10), E=(1L<<11), U=(1L<<12),
  fF=(1L<<13), fR=(1L<<14), fP=(1L<<15), fB=(1L<<16), fL=(1L<<17),
  fG=(1L<<18), fT=(1L<<19), fS=(1L<<20), fD=(1L<<21), fZ=(1L<<22),

  INITIAL = STAR | iS | iT | iK | iP | iW | iH | iR,
  VOWELS = A | O | E | U,
  FINAL = STAR | fF | fR | fP | fB | fL | fG | fT | fS | fD | fZ,
  WHOLE = NUM | INITIAL | VOWELS | FINAL,

  n0 = NUM | O,
  n1 = NUM | iS,
  n2 = NUM | iT,
  n3 = NUM | iP,
  n4 = NUM | iH,
  n5 = NUM | A,
  n6 = NUM | fF,
  n7 = NUM | fP,
  n8 = NUM | fL,
  n9 = NUM | fT,

  iB = iP | iW,
  iCH = iK | iH,
  iD = iT | iK,
  iF = iT | iP,
  iG = iT | iK | iP | iW,
  iJ = iS | iK | iW | iR,
  iL = iH | iR,
  iM = iP | iH,
  iN = iT | iP | iH,
  iQU = iK | iW,
  iV = iS | iR,
  iY = iK | iW | iR,
  iZ = iS | STAR,

  fCH = fF | fP,
  fJ = fP | fB | fL | fG,
  fK = fB | fG,
  fM = fP | fL,
  fMP = fF | fR | fP,
  fN = fP | fB,
  fNG = fP | fB | fG,
  fNK = STAR | fP | fB | fG,
  fRCH = fF | fR | fP | fB,
  fRF = fF | fR | fB,
  fSH = fR | fB,
  fV = STAR | fF,
  fX = fB | fG | fS,
  fSHUN = fG | fS,
  fKSHUN = STAR | fB | fG | fS,
  
  COMMA = fR | fB | fG | fS,
  CORRECTION = STAR,
  PARAGRAPH = iP | fF,
  PERIOD = fF | fP | fL | fT,
  QUESTION = iS | iT | iP | iH,
  NEWLINE = iP | iR | A | fF,

  AY = A | E | U,
  AW = A | U,
  EE = A | O | E,
  I = E | U,
  IY = A | O | E | U,
  OH = O | E,
  OI = O | E | U,
  OO = A | O,
  OU = O | U,
  UU = A | O | U,
};
static char button_name[] = "#STKPWHRAO*EUFRPBLGTSDZ";

#define REPLACE(part, val, str) \
  if ((ch & (part)) == (val)) { \
    ch &= ~(part); \
    strcpy(out + pos, str); \
    pos += strlen(str); \
  }

void steno_to_string(long ch, char *out) {
  int pos = 0;

  REPLACE(WHOLE, COMMA, ",");
  REPLACE(WHOLE, CORRECTION, "[correction]");
  REPLACE(WHOLE, PARAGRAPH, "[paragraph]");
  REPLACE(WHOLE, PERIOD, ".");
  REPLACE(WHOLE, QUESTION, "?");
  REPLACE(WHOLE, NEWLINE, "[newline]");

  REPLACE(WHOLE, n0, "0");
  REPLACE(WHOLE, n1, "1");
  REPLACE(WHOLE, n2, "2");
  REPLACE(WHOLE, n3, "3");
  REPLACE(WHOLE, n4, "4");
  REPLACE(WHOLE, n5, "5");
  REPLACE(WHOLE, n6, "6");
  REPLACE(WHOLE, n7, "7");
  REPLACE(WHOLE, n8, "8");
  REPLACE(WHOLE, n9, "9");
  
  REPLACE(INITIAL, iB, "B");
  REPLACE(INITIAL, iCH, "CH");
  REPLACE(INITIAL, iD, "D");
  REPLACE(INITIAL, iF, "F");
  REPLACE(INITIAL, iG, "G");
  REPLACE(INITIAL, iJ, "J");
  REPLACE(INITIAL, iL, "L");
  REPLACE(INITIAL, iM, "M");
  REPLACE(INITIAL, iN, "N");
  REPLACE(INITIAL, iQU, "QU");
  REPLACE(INITIAL, iV, "V");
  REPLACE(INITIAL, iY, "Y");
  REPLACE(INITIAL, iZ, "Z");

  REPLACE(iS, iS, "S");
  REPLACE(iT, iT, "T");
  REPLACE(iK, iK, "K");
  REPLACE(iP, iP, "P");
  REPLACE(iW, iW, "W");
  REPLACE(iH, iH, "H");
  REPLACE(iR, iR, "R");

  REPLACE(VOWELS, AY, "AY");
  REPLACE(VOWELS, AW, "AW");
  REPLACE(VOWELS, EE, "EE");
  REPLACE(VOWELS, I, "I");
  REPLACE(VOWELS, IY, "IY");
  REPLACE(VOWELS, OH, "OH");
  REPLACE(VOWELS, OI, "OI");
  REPLACE(VOWELS, OO, "OO");
  REPLACE(VOWELS, OU, "OU");
  REPLACE(VOWELS, UU, "UU");

  REPLACE(A, A, "A");
  REPLACE(O, O, "O");
  REPLACE(STAR, STAR, "*");
  REPLACE(E, E, "E");
  REPLACE(U, U, "U");

  REPLACE(FINAL, fCH, "CH");
  REPLACE(FINAL, fJ, "J");
  REPLACE(FINAL, fK, "K");
  REPLACE(FINAL, fM, "M");
  REPLACE(FINAL, fMP, "MP");
  REPLACE(FINAL, fN, "N");
  REPLACE(FINAL, fNG, "NG");
  REPLACE(FINAL, fNK, "NK");
  REPLACE(FINAL, fRCH, "RCH");
  REPLACE(FINAL, fRF, "RF");
  REPLACE(FINAL, fSH, "SH");
  REPLACE(FINAL, fV, "V");
  REPLACE(FINAL, fX, "X");
  REPLACE(FINAL, fSHUN, "SHUN");
  REPLACE(FINAL, fKSHUN, "KSHUN");

  REPLACE(fF, fF, "F");
  REPLACE(fR, fR, "R");
  REPLACE(fP, fP, "P");
  REPLACE(fB, fB, "B");
  REPLACE(fL, fL, "L");
  REPLACE(fG, fG, "G");
  REPLACE(fT, fT, "T");
  REPLACE(fS, fS, "S");
  REPLACE(fD, fD, "D");
  REPLACE(fZ, fZ, "Z");
  out[pos] = '\0';
}

void steno_to_string_simple(long ch, char *out) {  
  int i, pos;
  pos = 0;
  for (i = 0; i < CAPTOUCH_BUTTONS; i++) {
    if (ch & 1) {
      out[pos++] = button_name[i];
    }
    ch >>= 1;
  }
  out[pos] = '\0';
}