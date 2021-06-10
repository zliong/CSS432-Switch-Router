#include <iostream>
#include <cstdint>
#include <iterator>
#include <map>
#include <random>

using namespace std;

struct EthernetPacket{
    uint8_t dest[6];
    uint8_t src[6];
    unsigned int type: 2;
    unsigned int DSAP: 1;   //destination service access point
    unsigned int SSAP: 1;   //source service access point
    unsigned int checksum: 4;
    uint32_t controlfield: 1;
    void* data ;
};

struct Entry{
    uint8_t addr[6];
    int port;
    string strAddr;
};
struct Switch
{
    Entry table[11];
    int length;
};

bool checkSwitchContent(uint8_t dest[], uint8_t cur[])
{
    uint32_t mask = 128;  //in binary 10000000
    for(int k = 0; k <= 5; k++)
    {
        for(int j = 0; j <= 7; j++)
        {
            if((cur[k] & mask) == (dest[k] & mask))
            {
                mask = mask >> 1;   //shift to right by 1 bit
            }
            else
            {
                return false;
            }
        }
        mask = 128;   //reset it back to default value
    }
    return true;    //means that it is exactly matching and exists in the switch
}

int checkSwitchTable(EthernetPacket packet, Switch* ptr)
{
    //Entry* temp = ptr->node;
    for(int i = 0; i < 12; i++)
    {
        if(checkSwitchContent(packet.dest, ptr->table[i].addr))
        {
            return ptr->table[i].port;  //return the interface/port it was found
        }
    }

    return 0;   //Did not find the exact MAC address, broadcast to ALL and add it to the table
}

bool AddEntry(EthernetPacket packet, Switch* ptr)
{
    int length = ptr->length;
    for(int i = 0; i < ptr->length; i++)
    {
        if(checkSwitchContent(packet.src, ptr->table[i].addr))
        {
            return false;
        }
    }
    if(ptr->length < 11)
    {
        Entry newEnt;
        ptr->table[length].addr[0] = packet.src[0];
        ptr->table[length].addr[1] = packet.src[1];
        ptr->table[length].addr[2] = packet.src[2];
        ptr->table[length].addr[3] = packet.src[3];
        ptr->table[length].addr[4] = packet.src[4];
        ptr->table[length].addr[5] = packet.src[5];
        ptr->table[length].port = length + 1;
        ptr->length += 1;
        return true;
    }
    else
    {
        cout << "Switch Table is now at full capacity!!" << endl;
        return false;
    }
}

void initializeSwitchTable(Switch* ptr)
{
    ptr->length = 4;
    ptr->table[0].addr[0] = 253;
    ptr->table[0].addr[1] = 240;
    ptr->table[0].addr[2] = 180;
    ptr->table[0].addr[3] = 225;
    ptr->table[0].addr[4] = 0;
    ptr->table[0].addr[5] = 0;
    ptr->table[0].port = 1;
    ptr->table[0].strAddr = "FD-F0-B4-E1-00-00";

    ptr->table[1].addr[0] = 20;
    ptr->table[1].addr[1] = 123;
    ptr->table[1].addr[2] = 240;
    ptr->table[1].addr[3] = 225;
    ptr->table[1].addr[4] = 0;
    ptr->table[1].addr[5] = 1;
    ptr->table[1].port = 2;
    ptr->table[1].strAddr = "14-7B-F0-E1-00-01";

    ptr->table[2].addr[0] = 121;
    ptr->table[2].addr[1] = 43;
    ptr->table[2].addr[2] = 180;
    ptr->table[2].addr[3] = 225;
    ptr->table[2].addr[4] = 195;
    ptr->table[2].addr[5] = 0;
    ptr->table[2].port = 3;
    ptr->table[2].strAddr = "79-2B-B4-E1-C3-00";

    ptr->table[3].addr[0] = 194;
    ptr->table[3].addr[1] = 240;
    ptr->table[3].addr[2] = 180;
    ptr->table[3].addr[3] = 225;
    ptr->table[3].addr[4] = 190;
    ptr->table[3].addr[5] = 0;
    ptr->table[3].port = 4;
    ptr->table[3].strAddr = "C2-F0-B4-E1-BE-00";
}
void displaySwitchingTable(Switch *ptr)
{
    cout << "MAC Address                                     Interface" << endl;
    cout << "---------------------------------------------------------" << endl;
    for(int i = 0; i < ptr->length;i++)
    {
        //cout << ptr->table[i].strAddr << "                                       " << ptr->table[i].port << endl;
        if(i >= 4)
        {
            cout <<unsigned(ptr->table[i].addr[0]) << "-" << unsigned(ptr->table[i].addr[1]) << "-"  << unsigned(ptr->table[i].addr[2]) << "-" << unsigned(ptr->table[i].addr[3]) << "-"  << unsigned(ptr->table[i].addr[4]) << "-" 
            << unsigned(ptr->table[i].addr[5]) << "                                        " << ptr->table[i].port << endl;
        }
        else{
            cout << ptr->table[i].strAddr << "                                       " << ptr->table[i].port << endl;
        }
    }
    // cout << ptr->table[0].strAddr << "                                       " << ptr->table[0].port << endl;
    // cout << ptr->table[1].strAddr << "                                       " << ptr->table[1].port << endl;
    // cout << ptr->table[2].strAddr << "                                       " << ptr->table[2].port << endl;
    // cout << ptr->table[3].strAddr << "                                       " << ptr->table[3].port << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << endl << endl;
}

void printFrame(EthernetPacket pkt, int packetNum, int port)
{
    cout << std::hex << "Packet[" << packetNum << "] Frame from " << unsigned(pkt.src[0]) << "-" <<  unsigned(pkt.src[1]) << "-"  << unsigned(pkt.src[2]) << "-" << unsigned(pkt.src[3]) << "-"  << unsigned(pkt.src[4]) << "-" << unsigned(pkt.src[5]) << " to " 
    << unsigned(pkt.dest[0]) << "-" << unsigned(pkt.dest[1]) << "-"  << unsigned(pkt.dest[2]) << "-" << unsigned(pkt.dest[3]) << "-"  << unsigned(pkt.dest[4]) << "-" << unsigned(pkt.dest[5]) << " was switched onto interface " << port << "." << endl;
}
void comparePacket(EthernetPacket pkt, Switch* table, int packetNum)
{
    int port = checkSwitchTable(pkt, table);
    //printFrame(pkt, packetNum, port);
    if(port == 0)
    {
        printFrame(pkt, packetNum, 1);
        cout << "Packet[" << packetNum <<"]  was broadcasted on all interfaces." << endl; 
    }
    else
    {
        printFrame(pkt, packetNum, port);
    }
    if(AddEntry(pkt, table))
    {
        cout << std::hex << "Packet[" << packetNum << "] Source address " <<  unsigned(pkt.src[0]) << "-" << unsigned(pkt.src[1]) << "-"  << unsigned(pkt.src[2]) << "-" << unsigned(pkt.src[3]) << "-"  << unsigned(pkt.src[4]) << "-" 
        << unsigned(pkt.src[5]) << " was added to switch interface" << endl;
    }
    cout << endl << endl;
}
int main()
{
    Switch stable;
    initializeSwitchTable(&stable);
    displaySwitchingTable(&stable);

    //Create the packets
    EthernetPacket pkt[5];
    //Create Packet with unknown destination
    pkt[0].src[0] = 253;
    pkt[0].src[1] = 240;
    pkt[0].src[2] = 180;
    pkt[0].src[3] = 225;
    pkt[0].src[4] = 0;
    pkt[0].src[5] = 0;
    pkt[0].type = 2;
    pkt[0].checksum = 12;
    pkt[0].DSAP = 1;
    pkt[0].SSAP = 1;
    pkt[0].dest[0] = 131;
    pkt[0].dest[1] = 43;
    pkt[0].dest[2] = 180;
    pkt[0].dest[3] = 225;
    pkt[0].dest[4] = 190;
    pkt[0].dest[5] = 0;
    //Create Packet with no issues
    pkt[1].src[0] = 121;
    pkt[1].src[1] = 43;
    pkt[1].src[2] = 180;
    pkt[1].src[3] = 225;
    pkt[1].src[4] = 195;
    pkt[1].src[5] = 0;
    pkt[1].type = 3;
    pkt[1].checksum = 15;
    pkt[1].DSAP = 1;
    pkt[1].SSAP = 1;
    pkt[1].dest[0] = 20;
    pkt[1].dest[1] = 123;
    pkt[1].dest[2] = 240;
    pkt[1].dest[3] = 225;
    pkt[1].dest[4] = 0;
    pkt[1].dest[5] = 1;
    //Create a packet that src should be added to switch table
    pkt[2].src[0] = 100;
    pkt[2].src[1] = 46;
    pkt[2].src[2] = 112;
    pkt[2].src[3] = 250;
    pkt[2].src[4] = 195;
    pkt[2].src[5] = 0;
    pkt[2].type = 3;
    pkt[2].checksum = 15;
    pkt[2].DSAP = 1;
    pkt[2].SSAP = 1;
    pkt[2].dest[0] = 20;
    pkt[2].dest[1] = 123;
    pkt[2].dest[2] = 240;
    pkt[2].dest[3] = 225;
    pkt[2].dest[4] = 0;
    pkt[2].dest[5] = 1;
    //Create a packet that its destination is not known and must be broadcasted
    pkt[3].src[0] = 194;
    pkt[3].src[1] = 240;
    pkt[3].src[2] = 180;
    pkt[3].src[3] = 225;
    pkt[3].src[4] = 190;
    pkt[3].src[5] = 0;
    pkt[3].type = 3;
    pkt[3].checksum = 15;
    pkt[3].DSAP = 1;
    pkt[3].SSAP = 1;
    pkt[3].dest[0] = 10;
    pkt[3].dest[1] = 193;
    pkt[3].dest[2] = 245;
    pkt[3].dest[3] = 235;
    pkt[3].dest[4] = 0;
    pkt[3].dest[5] = 1;
    //Create a packet that destination is on interface 3 with the added packet
    pkt[4].src[0] = 100;
    pkt[4].src[1] = 46;
    pkt[4].src[2] = 112;
    pkt[4].src[3] = 250;
    pkt[4].src[4] = 195;
    pkt[4].src[5] = 0;
    pkt[4].type = 3;
    pkt[4].checksum = 15;
    pkt[4].DSAP = 1;
    pkt[4].SSAP = 1;
    pkt[4].dest[0] = 121;
    pkt[4].dest[1] = 43;
    pkt[4].dest[2] = 180;
    pkt[4].dest[3] = 225;
    pkt[4].dest[4] = 195;
    pkt[4].dest[5] = 0;

    for(int i = 0; i < 5; i++)
    {
        comparePacket(pkt[i], &stable, i);
    }
    
    //re-display with added Interface
    displaySwitchingTable(&stable);
}