/** 
Converts quint list (dash separated) of arbitrary length to corresponding hex dump prefixed with x
Quint list must contain pairs of quints to align with 32 bit boundary
*/
function quint2hex(inp){
  var ret="x";
  var remaining=inp;
  while (remaining.length>0){
    var current=remaining.substring(0,11);
    if (! current.match(/^[bdfghjklmnprstvzaiou]{5}-[bdfghjklmnprstvzaiou]{5}$/) ) {
      console.err("bad quint format detected. expected format cvcvc-cvcvc - found: "+current);
      throw "bad quint format for: "+inp;
    }
    if ( remaining.length>12 ) remaining=remaining.substring(12);
    else remaining=remaining.substring(11);
    var currentHex=quint2uint(current).toString(16);
    while(currentHex.length<8) currentHex="0"+currentHex;
    ret+=currentHex;
  }
  return ret;
}

/** 
Converts hex dump (x prefixed) to a list of quints. Hex dump must contain leading zeros when necessary.
Hex dump length must by divisible by 8.
*/
function hex2quint(hex){
  var sep="-"; 
  var ret="";
  if (! inp.value.match(/^x(([0-9a-f]{2}){2})+$/)) {
      console.err("Bad hex length: "+hex.length);
      throw ("Bad hex length: "+hex.length);
  }
  var remaining = hex.substring(1); //skip x
  while (remaining.length>0){
    if (remaining.length<8 && remaining.length%8!=0 ) {
      console.err("Bad hex length: "+remaining.length);
      throw "Bad hex length: "+hex;
    }
    var current=remaining.substring(0,8);
    remaining=remaining.substring(8);
    var currentQuint=uint2quint(parseInt(current,16)>>0);
    ret+=currentQuint;
    if ( sep ) ret+=sep;
  }
  if (ret.endsWith(sep)) ret=ret.substring(0,ret.length-sep.length);
  return ret;
}


//following code is based on original java impl.

var uint2consonant = [
    'b', 'd', 'f', 'g',
    'h', 'j', 'k', 'l',
    'm', 'n', 'p', 'r',
    's', 't', 'v', 'z'
];

var uint2vowel = [
    'a', 'i', 'o', 'u'
];


var MASK_FIRST4 = 0xF0000000;
var MASK_FIRST2 = 0xC0000000;

function uint2quint(i) {
    var j;
    var quint="";
    
    j = i & MASK_FIRST4; i <<= 4; j >>>= 28; quint+=(uint2consonant[j]);
    j = i & MASK_FIRST2; i <<= 2; j >>>= 30; quint+=(uint2vowel[j]);
    j = i & MASK_FIRST4; i <<= 4; j >>>= 28; quint+=(uint2consonant[j]);
    j = i & MASK_FIRST2; i <<= 2; j >>>= 30; quint+=(uint2vowel[j]);
    j = i & MASK_FIRST4; i <<= 4; j >>>= 28; quint+=(uint2consonant[j]);

    quint+="-";

    j = i & MASK_FIRST4; i <<= 4; j >>>= 28; quint+=(uint2consonant[j]);
    j = i & MASK_FIRST2; i <<= 2; j >>>= 30; quint+=(uint2vowel[j]);
    j = i & MASK_FIRST4; i <<= 4; j >>>= 28; quint+=(uint2consonant[j]);
    j = i & MASK_FIRST2; i <<= 2; j >>>= 30; quint+=(uint2vowel[j]);
    j = i & MASK_FIRST4; i <<= 4; j >>>= 28; quint+=(uint2consonant[j]);
	  return quint;
}

function quint2uint(quint) {
    var res = 0;
    var remaining = quint.length;
    var i=0;
    while(remaining>0) {
      var c=quint[i++];
		  remaining --; 
      switch(c) {

        /* consonants */
      case 'b': res <<= 4; res +=  0; break;
      case 'd': res <<= 4; res +=  1; break;
      case 'f': res <<= 4; res +=  2; break;
      case 'g': res <<= 4; res +=  3; break;

      case 'h': res <<= 4; res +=  4; break;
      case 'j': res <<= 4; res +=  5; break;
      case 'k': res <<= 4; res +=  6; break;
      case 'l': res <<= 4; res +=  7; break;

      case 'm': res <<= 4; res +=  8; break;
      case 'n': res <<= 4; res +=  9; break;
      case 'p': res <<= 4; res += 10; break;
      case 'r': res <<= 4; res += 11; break;

      case 's': res <<= 4; res += 12; break;
      case 't': res <<= 4; res += 13; break;
      case 'v': res <<= 4; res += 14; break;
      case 'z': res <<= 4; res += 15; break;

        /* vowels */
      case 'a': res <<= 2; res +=  0; break;
      case 'i': res <<= 2; res +=  1; break;
      case 'o': res <<= 2; res +=  2; break;
      case 'u': res <<= 2; res +=  3; break;

        /* separators */
      default: break;
      }
    }
    return res>>>0; //the unsigned right shift fixes signed int issue - http://stackoverflow.com/a/17106974/1777150 
}

