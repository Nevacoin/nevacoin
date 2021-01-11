// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 500;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
         ( 0,   uint256("0x00000e4b9ec4fef5b26bc05a8a50b999fd312ee6f8bf324371ffd790c2961988") )
         ( 50000,   uint256("0x00000000000343953257931cfa5238b192d183d3369521853f2023cb3a2f44c3") )
         ( 100000,   uint256("0x000000000026d9a62e799b0c1d132a862ac8c9bbc2a617c249d187bdf5cb7f75") )
         ( 150001,   uint256("0000000000c9468f2f26da34be61d45a5d4e7e3d0ed7c0db134f58f34c637e17") )
         ( 177777,   uint256("00000000011bf19a785fdd113826634110df06106896d5b40bc7fba41f136173") )
         ( 192525,   uint256("000000000002c09a6213e14f7a0edc57cb13e21e73556b1be66d724cca0de71a") )
         ( 219999,   uint256("000000000040745edd90edfc5f858ddd7f13dc87c3136dbd4ac153d0b9ac927c") )
         ( 234650,   uint256("00000000000669d4dd6bc230c6aa9b85049167b4be7f57803a02056a4f09706a") )
         ( 300000,   uint256("0x00000000000d6b01b6728d72c118ff105d5ed01d97932d74c9c476d33079d6db") )
         ( 400000,   uint256("0x5cf39f3b6d6951ab7ef9311be6b77912aa309e8bb5e33d72a89803f7d5d2530e") )
         ( 500000,   uint256("0x00000000000f61bb09efbd9886ae7916314963653af98e5fea84f5b51e8c7539") )
         ( 600000,   uint256("0x00000000000082924663054c6f80f9fcb8bc5d417db593e690afd470dfcd5bba") )
         ( 700000,   uint256("0x000000000000afa8ddbde4ac97dc36b93d8cd59fe59d7d25818bbd3ae44d54c0") )
         ( 800000,   uint256("0x000000000000255c9d4a71aaf1ca0bfd63db1468bd8da7bc6cf8dfcc6e98b192") )
         ( 969000,   uint256("0x00000000005097f7feffe94f20f4a7a7dcc3b8aac9c04c019c6bb2490b24bfbe") )
         ( 1023487,  uint256("0x5b47448be12c80ee0cf883a46ec21a1840e6ccc70e1d674d49596c93535f5f30") )
         ( 1062544,  uint256("0x442443016572e89610e00a8e898b29114c515304730eef6cbd9d3189184a8644") )
         ( 1077654,  uint256("0x04ad81718a369ff59a219ff0dc5870215009bf3f3361d9f76962c72b8a6065d8") )
         ( 1085180,  uint256("0x152b697039d1aa9d08bb33ebf0f9316c1ec0824145ec628e5ce298fd40809ee5") )
         ( 1081111,  uint256("0x73ce0a825995bcf3acef051a0366dd7716e5264771f438003b06a5dcda00596f") )


    ;


    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();

        if (nHeight <= pindexSync->nHeight)
            return false;
        return true;
    }
}
