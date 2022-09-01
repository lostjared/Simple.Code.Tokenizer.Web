#include<emscripten.h>
#include<emscripten/bind.h>

#include"lexer.hpp"

#include<sstream>

std::string toHTML(const std::string &text) {
    std::string temp;
    for(unsigned int i = 0; i < text.length(); ++i) {
        switch(text[i]) {
            case '<':
                temp += "&lt;";
                break;
            case '>':
                temp += "&gt;";
                break;
            case '(':
                temp += "&#40;";
                break;
            case ')':
                temp += "&#41;";
                break;
            case '.':
                temp += "&#46;";
                break;
            case ';':
                temp += "&#59;";
                break;
            default:
                temp += text[i];
                break;
        }
    }
    return temp;
}

int counter = 0;

void outputToken(std::ostringstream &out, lex::Token &t) {
    if(t.isKeyword() && (t.getTokenType() != lex::TOKEN_STRING && t.getTokenType() != lex::TOKEN_SINGLE)) {
        out << "<tr><th class=\"lineindex_color\">" << ++counter << "</th><th class=\"codekeyword\">" << toHTML(t.getToken()) << "</b></th><th class=\"linetype\">Keyword</th><th class=\"linenum\">" << t.line << "</th></tr>";
    } else {
        std::string codecolor;
        switch(t.getTokenType()) {
            case lex::TOKEN_STRING:
                codecolor = "codestring";
                break;
            case lex::TOKEN_OPERATOR:
                codecolor = "codesymbol";
                break;
            case lex::TOKEN_DIGIT:
                codecolor = "codedigit";
                break;
            case lex::TOKEN_SINGLE:
                codecolor = "codechar";
                break;
            default:
                codecolor = "codetext";
        }
        out << "<tr><th class=\"lineindex_color\">" << ++counter << "</th><td class=\"" << codecolor << "\">" << toHTML(t.getToken()) << "</th><th class=\"linetype\">" << t.getType() << "</th><th class=\"linenum\">" << t.line << "</th></tr>\n";
    }
}


std::string urldecode(std::string text) {
    std::ostringstream stream;
    for(unsigned int i = 0; i < text.length(); ++i) {
        if(text[i] == '+') {
            stream << " ";
            continue;
        }
        if(text[i] == '%') {
            if(i+2 < text.length()) {
                ++i;
                std::string test;
                test += text[i];
                ++i;
                test += text[i];
                int char_value;
                sscanf(test.c_str(), "%x", &char_value);
                stream << (char)char_value;
                continue;
            }
        }
        stream << text[i];
    }
    return stream.str();
}


class ScanExport {
public:

    ScanExport() {}

    std::string scanLex(std::string ctext) {
        counter = 0;
        std::ostringstream ofile;
        std::istringstream input(ctext);
        lex::Scanner scan(input);
        ofile << "<table border=\"1\" cellspacing=\"4\" cellpadding=\"4\"><tr style=\"background-color: rgb(150, 150, 150);\"><th class=\"linenumber\"><b>Index</b></th><th class=\"linenumber\"><b>Token</b></th><th class=\"linenumber\"><b>Type</b></th><td class=\"linenumber\"><b>Line Number</b></th></tr>\n";
        try {
            while(scan.valid()) {
                lex::Token token;
                scan >> token;
                outputToken(ofile, token);
            }
        } catch(lex::Scanner_EOF) {
        
        } catch(lex::Scanner_Error) {
            std::cout << "A error has occured. Check your source.\n";
        }
        ofile << "\n</table>";
        return ofile.str();
    }

};

using namespace emscripten;

EMSCRIPTEN_BINDINGS(my_Scan) {
    class_<ScanExport>("Scan")
        .constructor()
        .function("scanLex", &ScanExport::scanLex)
    ;
}