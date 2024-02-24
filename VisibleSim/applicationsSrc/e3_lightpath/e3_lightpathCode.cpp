/**
 * @file e3_LightPathCode.cpp
 * Generated by VisibleSim BlockCode Generator
 * https://services-stgi.pu-pm.univ-fcomte.fr/visiblesim/generator.php#
 * @author yourName
 * @date 2022-09-20
 **/
#include "e3_LightPathCode.hpp"

E3_LightPathCode::E3_LightPathCode(BlinkyBlocksBlock *host):BlinkyBlocksBlockCode(host),module(host) {
    // @warning Do not remove block below, as a blockcode with a NULL host might be created
    //  for command line parsing
    if (not host) return;

    // Registers a callback (myBroadcastFunc) to the message of type R
    addMessageEventFunc2(GO_MSG_ID,
                       std::bind(&E3_LightPathCode::myGoFunc,this,
                       std::placeholders::_1, std::placeholders::_2));

    // Registers a callback (myForecastFunc) to the message of type R
    addMessageEventFunc2(BACK_MSG_ID,
                       std::bind(&E3_LightPathCode::myBackFunc,this,
                       std::placeholders::_1, std::placeholders::_2));

    // Registers a callback (myForecastFunc) to the message of type R
    addMessageEventFunc2(LIGHT_MSG_ID,
                         std::bind(&E3_LightPathCode::myLightFunc,this,
                                   std::placeholders::_1, std::placeholders::_2));

}

void E3_LightPathCode::startup() {
    console << "start " << getId() << "\n";
    myDistance=10000;
    myParent=nullptr;
    myLongestChildDist=0;
    myLongestChild=nullptr;
    if (isLeader) {
        setColor(RED);
        myNbWaitedAnswer=sendMessageToAllNeighbors("go",
                                  new MessageOf<int>(GO_MSG_ID,1), 100, 1000, 0);
    }
}

void E3_LightPathCode::myGoFunc(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender) {
    MessageOf<int>* msg = static_cast<MessageOf<int>*>(_msg.get());
    int msgData = *msg->getData();

    console << "rcv " << msgData << " from " << sender->getConnectedBlockBId() << "\n";
    if (myParent==nullptr) {
        myDistance = msgData;
        myParent=sender;
        setColor(Color(0.8f,0.8f,0.8f));
        myNbWaitedAnswer=sendMessageToAllNeighbors("go",
                                  new MessageOf<int>(GO_MSG_ID,msgData + 1), 100, 1000, 1, sender);
        if (myNbWaitedAnswer==0) {
            sendMessage("back",new MessageOf<int>(BACK_MSG_ID,myDistance),myParent,100,1000);
        }
    } else {
        sendMessage("back",new MessageOf<int>(BACK_MSG_ID,-1),sender,100,1000);
    }
}

void E3_LightPathCode::myBackFunc(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender) {
    MessageOf<int>* msg = static_cast<MessageOf<int>*>(_msg.get());
    int msgData = *msg->getData();
    console << "rcv ACK " << msgData << " from " << sender->getConnectedBlockBId() << "\n";

    if (msgData>myLongestChildDist) {
        myLongestChildDist=msgData;
        myLongestChild=sender;
    }

    myNbWaitedAnswer--;
    if (myNbWaitedAnswer==0) {
        if (myParent) {
            sendMessage("back", new MessageOf<int>(BACK_MSG_ID,max(myLongestChildDist,myDistance)), myParent, 100, 1000);
        } else { // Leader
            setColor(RED);
            sendMessage("Light", new Message(LIGHT_MSG_ID),myLongestChild,100,0);
        }
    }
}

void E3_LightPathCode::myLightFunc(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender) {
    setColor(myDistance);
    if (myLongestChild) {
        sendMessage("Light", new Message(LIGHT_MSG_ID),myLongestChild,100,0);
    }
}


void E3_LightPathCode::parseUserBlockElements(TiXmlElement *config) {
    const char *attr = config->Attribute("leader");
    if (attr!=nullptr) {
        std::cout << getId() << " is leader!" << std::endl; // complete with your code
        isLeader=true;
    }
}