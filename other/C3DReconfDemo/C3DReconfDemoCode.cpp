
/**
 * @file C3DReconfDemoCode.cpp
 * Generated by VisibleSim BlockCode Generator
 * https://services-stgi.pu-pm.univ-fcomte.fr/visiblesim/generator.php#
 * @author yourName
 * @date 2024-01-27
 **/

#include "C3DReconfDemoCode.hpp"
#include "robots/catoms3D/catoms3DMotionEngine.h"
#include "robots/catoms3D/catoms3DRotationEvents.h"

C3DReconfDemoCode::C3DReconfDemoCode(Catoms3DBlock *host) : Catoms3DBlockCode(host), module(host)
{
    // @warning Do not remove block below, as a blockcode with a NULL host might be created
    //  for command line parsing
    if (not host)
        return;

    // Registers a callback (myGoFunc) to the message of type O
    addMessageEventFunc2(GO_MSG_ID,
                         std::bind(&C3DReconfDemoCode::myGoFunc, this,
                                   std::placeholders::_1, std::placeholders::_2));

    // Registers a callback (myBackFunc) to the message of type C
    addMessageEventFunc2(BACK_MSG_ID,
                         std::bind(&C3DReconfDemoCode::myBackFunc, this,
                                   std::placeholders::_1, std::placeholders::_2));

    // Registers a callback (myWakeupFunc) to the message of type E
    addMessageEventFunc2(WAKEUP_MSG_ID,
                         std::bind(&C3DReconfDemoCode::myWakeupFunc, this,
                                   std::placeholders::_1, std::placeholders::_2));

    addMessageEventFunc2(ACKNOWLEDGE_MSG_ID,
                         std::bind(&C3DReconfDemoCode::myAcknowledgeFunc,
                                   this,
                                   std::placeholders::_1,
                                   std::placeholders::_2));

    // Registers a callback (myGoFunc) to the message of type O
    addMessageEventFunc2(BROADCAST_MSG_ID,
                         std::bind(&C3DReconfDemoCode::myBroadcastFunc, this,
                                   std::placeholders::_1, std::placeholders::_2));
}

void C3DReconfDemoCode::startup()
{
    console << "start " << getId() << "\n";
    // console << "isInTarget" << target->isInTarget(module->position) << "\n";

    if (isLeader)
    {
        module->setColor(RED);
        distance = 0;
        nbWaitedAnswers = sendMessageToAllNeighbors("distance", new MessageOf<int>(BROADCAST_MSG_ID, distance + 1), 1000, 100, 0);
    }
}
void C3DReconfDemoCode::myGoFunc(std::shared_ptr<Message> _msg, P2PNetworkInterface *sender)
{
    MessageOf<int> *msg = static_cast<MessageOf<int> *>(_msg.get());
    int msgData = *msg->getData();
}

void C3DReconfDemoCode::myBackFunc(std::shared_ptr<Message> _msg, P2PNetworkInterface *sender)
{
    // MessageOf<int>* msg = static_cast<MessageOf<int>*>(_msg.get());
    // int msgData = *msg->getData();
}

void C3DReconfDemoCode::myWakeupFunc(std::shared_ptr<Message> _msg, P2PNetworkInterface *sender)
{
}

void C3DReconfDemoCode::myAcknowledgeFunc(std::shared_ptr<Message> _msg, P2PNetworkInterface *sender)
{

    // ? la fonction Acknowledge sert à retourner le message de confirmation de la réception du message broadcast

    nbWaitedAnswers--; // on diminue le nombre de messages attendus, en effet on a reçu la confirmation d'un module
    console << "rec. Ack(" << nbWaitedAnswers << ") from " << sender->getConnectedBlockId() << "";
    if (nbWaitedAnswers == 0)
    { // si on a reçu toutes les confirmations
        if (parent == nullptr)
        { // si on est le leader
            setColor(-1);
        }
        else
        { // sinon on envoie un message de confirmation au parent
            sendMessage("ack2parent", new Message(ACKNOWLEDGE_MSG_ID), parent, 1000, 100);
        }
    }
}

void C3DReconfDemoCode::myBroadcastFunc(std::shared_ptr<Message> _msg, P2PNetworkInterface *sender)
{

    // ? la fonction Broadcast sert à faire passer le message pour transmettre la distance dans le but de réaliser le gradient

    MessageOf<int> *msg = static_cast<MessageOf<int> *>(_msg.get());
    int msgData = *msg->getData();

    if (parent == nullptr || msgData < distance)
    {
        distance = msgData;

        console << "module in target" << target->isInTarget(module->position) << "\n";
        vector<std::pair<const Catoms3DMotionRulesLink*, Catoms3DRotation>> motions = Catoms3DMotionEngine::getAllRotationsForModule(module);

        if (target->isInTarget(module->position))
        {
            setColor(RED);
        }
        else if (motions.size() > 0 && (canMove(1) || canMove(0) || canMove(7)) && module->hasNeighbor(8))
        {
            
            setColor(-1);
        }
        else
        {
            setColor(distance);
        }

        parent = sender;
        string str = "distance(";
        nbWaitedAnswers = sendMessageToAllNeighbors(str.c_str(), new MessageOf<int>(BROADCAST_MSG_ID, distance + 1), 1000, 100, 1, sender);
        if (nbWaitedAnswers == 0)
        {
            sendMessage("ack2parent", new Message(ACKNOWLEDGE_MSG_ID), parent, 1000, 100);
        }
    }
    else
    {
        sendMessage("ack2sender", new Message(ACKNOWLEDGE_MSG_ID), sender, 1000, 100);
    }
}

void C3DReconfDemoCode::parseUserBlockElements(TiXmlElement *config)
{
    const char *attr = config->Attribute("leader");
    isLeader = (attr ? Simulator::extractBoolFromString(attr) : false);
    if (isLeader)
    {
        std::cout << getId() << " is leader!" << std::endl; // complete with your code
    }
}

void C3DReconfDemoCode::onTap(int face)
{
    std::cout << "Block 'tapped':" << getId() << std::endl; // complete with your code here
}

int C3DReconfDemoCode::findNeighborPort(const Catoms3DBlock *neighbor) {
            int i=0;
  while (i<FCCLattice::MAX_NB_NEIGHBORS && module->getNeighborBlock(i)!=neighbor) {
            i++;
  }
  return (i<FCCLattice::MAX_NB_NEIGHBORS?i:-1);
}

bool C3DReconfDemoCode::canMove(int port) {
    vector<std::pair<const Catoms3DMotionRulesLink*, Catoms3DRotation>> motions =
            Catoms3DMotionEngine::getAllRotationsForModule(module);
   for (auto &motion:motions) {
       int pivotPort = findNeighborPort(motion.second.pivot);
       if (pivotPort==port) return true;
  }
  return false;
}
