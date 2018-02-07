/*****************************************************************************************************************
Copyright:
Author:Dou Menghan
Date:2017-11-21
Description:get quantum instruction
*****************************************************************************************************************/
#include "QuantumInstructionHandleAPI.h"
#include "X86QuantumGates.h"
#include "GPUQuantumGates.h"
#include "QuantumGateParameter.h"


#define CPU 1                                                           /* CPU CalculationUnit                  */
#define GPU 2                                                           /* GPU CalculationUnit                  */

/*****************************************************************************************************************
Name:        QuantumInstructionHandleAPI
Description: initialize the quantum gates
Argin:       None
Argout:      None
return:      None
*****************************************************************************************************************/
QuantumInstructionHandleAPI::QuantumInstructionHandleAPI()
{

}

/*****************************************************************************************************************
Name:        ~QuantumInstructionHandleAPI
Description: destroy the quantum gates
Argin:       None
Argout:      None
return:      None
*****************************************************************************************************************/
QuantumInstructionHandleAPI::~QuantumInstructionHandleAPI()
{

}


/*****************************************************************************************************************
Name:        transmit
Description: transmit the quantum instruction
Argin:       qInstructionList  quantum instruction list
             pQuantumProParam  quantum program param pointer
Argout:      None
return:      true or false
*****************************************************************************************************************/
bool QuantumInstructionHandleAPI::transmit(QList &qInstructionList,QuantumGateParam *pQuantumProParam)
{
    if (nullptr == pQuantumProParam)
    {
        return false;
    }

    QNode        *ptQNode = qInstructionList.qList.head->next;

    QuantumInstructionGet *pQuantumInstructionGet = QuantumInstructionGet::getInstance();

    if (NULL == mpQGate)
    {
        return false;
    }

    int  iNumber = 0;

    bool bResult = false;

    try
    {
        QNode endProg = { END,nullptr };
        if (!pQuantumInstructionGet->transmit(&endProg, pQuantumProParam, mpQGate))
        {
            return bResult;
        }
        
        /*
         * traverse QStat list
         */
        while (NULL != ptQNode)
        {
            iNumber++;

            if (!pQuantumInstructionGet->transmit(ptQNode, pQuantumProParam, mpQGate))
            {
                break;
            }
            ptQNode = ptQNode->next;
        }
    
        if (nullptr != ptQNode)
        {
            return bResult;
        }
    }
    catch (const std::exception& e)
    {
        cout << e.what() << endl;
        return false;
    }

    bResult = true;
    return bResult;
}


/*****************************************************************************************************************
Name:        initState
Description: init state
Argin:       pQuantumProParam quantum param
Argout:      pQuantumProParam quantum param
return:      true or false
*****************************************************************************************************************/
bool QuantumInstructionHandleAPI::initState(QuantumGateParam *pQuantumProParam)
{
    if (0 == pQuantumProParam->mQuantumBitNumber)
    {
        return false;
    }
    
    if (qErrorNone == mpQGate->initState(pQuantumProParam))
    {
        return true;
    }

    return false;
}

/*****************************************************************************************************************
Name:        destroyState
Description: destroyState state
Argin:       pQuantumProParam quantum param
Argout:      pQuantumProParam quantum param
return:      true or false
*****************************************************************************************************************/
bool QuantumInstructionHandleAPI::destroyState(QuantumGateParam *pQuantumProParam)
{
    if (nullptr == mpQGate)
    {
        return true;
    }
    if (qErrorNone != mpQGate->destroyState(pQuantumProParam->mQuantumBitNumber))
    {
        return false;
    }

    delete(mpQGate);
    mpQGate = nullptr;
    return true;
}

bool QuantumInstructionHandleAPI::getState(string & sState,QuantumGateParam *pQuantumProParam)
{
    return mpQGate->getQState(sState, pQuantumProParam);
}

/*****************************************************************************************************************
Name:        setComputeUnit
Description: set Calculation Unit 
Argin:       iCalculationUnitType Calculation Unit type
Argout:      None
return:      true or false
*****************************************************************************************************************/
bool QuantumInstructionHandleAPI::setComputeUnit(int iCalculationUnitType)
{
    if (nullptr != mpQGate)
    {
        cout << "compute unit has been initialized." << endl;
        return true;
    }

    if (CPU == iCalculationUnitType)
    {
        mpQGate = new X86QuantumGates;
        if (nullptr == mpQGate)
        {
            cout << "new X86QuantumGates fail" << endl;
            return false;
        }
    }
    else if (GPU == iCalculationUnitType)
    {
        try
        {
            mpQGate = new GPUQuantumGates;
        }
        catch (const std::exception&e)
        {
            cout << "new GPUQuantumGates fail" << endl;
            return false;
        }
    }
    
    return true;
}

