#include "LanguageString.h"

const LangArray_t stringsArray[Max_error_messages] = {
    {
        .LangArray[English] = "Temperature out of range",
        .LangArray[German] = "Temperatur außerhalb des Bereichs",
    },
    {
         .LangArray[English] = "State of Charge out of range",
         .LangArray[German] = "Ladezustand außerhalb des Bereichs",

    },
    {
         .LangArray[English] = "Charge Rate out of range",
         .LangArray[German] = "Laderate außerhalb des Bereichs",
    },
    {
         .LangArray[English] = "Breach_Low",
         .LangArray[German] = "Verletzung niedrig",
    },
    {
         .LangArray[English] = "Breach_High",
         .LangArray[German] = "Verletzung hoch",
    },
    {
         .LangArray[English] = "Approaching_discharge",
         .LangArray[German] = "Nähernde Entladung",
    },
    {
         .LangArray[English] = "Approaching charge-peak",
         .LangArray[German] = "Annäherung an die Ladungsspitze",
    },
    {
         .LangArray[English] =  "Temperature warning",
         .LangArray[German] = "Temperaturwarnung"
    },
    {
         .LangArray[English] =  "Soc Warning",
         .LangArray[German] = "soc Warnung"
    },
    {
         .LangArray[English] =  "Charge state warning",
         .LangArray[German] = "Ladezustandswarnung"
    },

};
