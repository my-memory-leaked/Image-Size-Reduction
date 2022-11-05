#include "pch.h"
#include "CppRLEEncode.h"


void RLEEncode( u8* input, u32 inputSize, u8* output ) {
    u8* inp = input;
    u8* out = output;
    u8* current_el = new u8;   // current elemed used to comparison
    u8 repetition_counter = 0; // number of repeted elements

    u32 itr = 0;
    for ( u32 i = 0; i < inputSize; i++, itr++ ) {
        *current_el = inp[i];   // new curreent element
        repetition_counter = 1; // we got first el

        while ( *current_el == inp[i + 1] ) {   // if element repets
            repetition_counter++;
            i++;
        }   // while ( *current_el == inp[i + 1] )

        out[itr] = repetition_counter;  // constructing output
        out[++itr] = *current_el;       // could be reversed
    }   // for ( u32 i = 0; i < inputSize; i++, itr++ )

    delete current_el;
}   // RLEEncode( u8* input, u32 inputSize, u8* output )
