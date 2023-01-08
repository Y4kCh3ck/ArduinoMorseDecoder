using namespace std;

char translate(String data) {
    
    // if error instatly return error 
    for( auto i : data ) 
      if (i == '!' || i == '?' ) return '!';
    
    // Variable for containg dot-dash series
    int buf = 0;
   
    // Variable for counting length of dot-dash series
    short i = 0;
    
    // Translate dot-shash series into binary -> 0 means DOT and 1 means DASH
    for ( ; i<data.length(); i++) {
        if(data[i] == '.')  buf |= (0<< i);       
        else if(data[i] == '-')  buf |= (1<< i);       
        else buf = 0xff;
    }

    // For debugging purposes
    // std::cout << std::bitset<8>(buf) << std::endl;    
    
    // DOT = 0; DASH = 1;    
    // NOTE: read binary representation backwards for example N = "-." = 0b01
    if( i == 1)
        switch(buf) {
            case 0b0: return 'E'; break;
            case 0b1: return 'T'; break;
        }
    else if( i == 2)
        switch(buf) {
            case 0b01: return 'N'; break;
            case 0b00: return 'I'; break;
            case 0b10: return 'A'; break;
            case 0b11: return 'M'; break;
        }
    else if(i == 3)
        switch(buf) {
            case 0b000: return 'S'; break;
            case 0b001: return 'D'; break;
            case 0b010: return 'R'; break;
            case 0b011: return 'G'; break;
            case 0b100: return 'U'; break;
            case 0b101: return 'K'; break;
            case 0b110: return 'W'; break;
            case 0b111: return 'O'; break;
        }
    else if(i == 4)
        switch(buf) {
            case 0b0000: return 'H'; break;
            case 0b0001: return 'B'; break;
            case 0b0010: return 'L'; break;
            case 0b0011: return 'Z'; break;
            case 0b0100: return 'F'; break;
            case 0b0101: return 'C'; break;
            case 0b0110: return 'P'; break;
            case 0b0111: return '?'; break;
            case 0b1000: return 'V'; break;
            case 0b1001: return 'X'; break;
            case 0b1010: return '?'; break;
            case 0b1011: return 'Q'; break;
            case 0b1100: return '?'; break;
            case 0b1101: return 'Y'; break;
            case 0b1110: return 'J'; break;
            case 0b1111: return '?'; break;
        }

    return '?';
}
