/** GUI related stuff*/
var inp=document.querySelector("#txtInput");
var out=document.querySelector("#txtOutput");
var btnConv=document.querySelector("#btnConvert");
var btnSwap=document.querySelector("#btnSwap");
var tt=document.querySelector("#toggle-theory");
var tho=document.querySelector("#theory");

btnConv.addEventListener("click", attemptConversion);
btnSwap.addEventListener("click", function (){ var x=inp.value; inp.value=out.value; out.value=x;});
tt.addEventListener("click", function (){
	tho.classList.toggle("hidden");
})

function attemptConversion(){
  console.log("attemptConversino");
  if (inp.value.match(/^([bdfghjklmnprstvzaiou]{5}-?)+$/)) out.value=quint2hex(inp.value);
  else if (inp.value.match(/^x(([0-9a-f]{2}){2})+$/)) out.value=hex2quint(inp.value);
  else 
     alert("Input format unsupported.");
}

