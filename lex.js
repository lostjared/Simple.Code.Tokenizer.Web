
var scanner;

var Module = {
    onRuntimeInitialized: function() {
        scanner = new Module.Scan();
    }
};


function Lex() {

    var text1 = document.getElementById("text1");
    var output = scanner.scanLex(text1.value);
    var divtag = document.getElementById("div2");
    divtag.innerHTML = output;
}