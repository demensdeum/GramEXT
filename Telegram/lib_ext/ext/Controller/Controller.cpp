#include "Controller.h"
#include <tiny-js/TinyJS.h>
#include "js_list.h"
#include <iostream>

using namespace GramExt;

CTinyJS *tinyJS = new CTinyJS();

const std::string signOutputMessageExtensionScript = R"(
    var signMessageTransformer = function(message) {
        print("Hello from signOutputMessageExtensionScript");
        message += "\n\nОтправлено из JavaScript расширения GramEXT";
        return message;
    };

    OutputMessageAddTransformer(signMessageTransformer);
)";

const std::string controllerScript = R"(
    var outputMessageTransformers = new List();

    var OutputMessageAddTransformer = function(transformer) {
        outputMessageTransformers.push(transformer);
    };

    var TransformOutputMessage = function(message) {
        var output = message;

        var transformFunc = function(transformer) {
            output = transformer(output);
        };

        outputMessageTransformers.forEach(transformFunc);

        print(output);

        global_returnValue = output;
    };
)";

void Controller::jsPrint(void *v, void *) {
    CScriptVar *var = (CScriptVar *) v;
    std::cout << var->getParameter("text")->getString() << std::endl;
}

void Controller::initialize() {
    tinyJS->addNative("function print(text)", reinterpret_cast<JSCallback>(Controller::jsPrint), nullptr);
    tinyJS->addNative("function console.log(text)", reinterpret_cast<JSCallback>(Controller::jsPrint), nullptr);
    runScript(R"(
        print("JS Initialized 1!");
        console.log("JS Initialized 2!");
    )");
    runScript(listScript);
    runScript(controllerScript);
    runScript(signOutputMessageExtensionScript);
}

std::string Controller::transformOutputText(const std::string text) {
    //return std::string(text) + "\n\nGramEXT_OutputTextTransformerTest";
    std::string output = runScript("TransformOutputMessage(\"" + text + "\");");
    return output;
}

std::string Controller::runScript(const std::string script) {
    std::string runScript = "var global_returnValue = \"\";" + script;
    std::cout << runScript << std::endl;
    tinyJS->execute(runScript.c_str());
    std::string output = tinyJS->evaluate("global_returnValue");
    std::cout << "global_returnValue: " << output << std::endl;
    return output;
}