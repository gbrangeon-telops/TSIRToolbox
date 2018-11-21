// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using PvDotNet;

namespace PvTransmitTiledImageSample
{
    /// <summary>
    /// Manage the buffers provided by the stream thread to only
    /// keep the last ( current ) one and return the other to the onwer. 
    /// On the other side, a snapshop can be taken by the transmitter thread
    /// at a given moment to be converted and transmitted.
    /// </summary>
    class CurrentBuffersTable
    {
#region constants
        public const UInt32 cMaxTileRows = 2;
        public const UInt32 cMaxTileColumns = 2;
#endregion

#region Constructor
        /// <summary>
        /// Constructor.
        /// </summary>
        public CurrentBuffersTable()
        {
            for (int i = 0; i < cMaxTileRows; i++)
            {
                for (int j = 0; j < cMaxTileColumns; j++)
                {
                    mCurrentTable[i, j] = null;
                    mSnapshot[i, j] = null;
                }
            }
        }
        #endregion

#region Data members
        /// <summary>
        /// Protect the access to the use count
        /// </summary>
        private Object mBufferTableObject = new object();

        /// <summary>
        /// This is the storage for the buffer. We keep the most recent one from each stream
        /// to be ready to provide a snapshot.
        /// </summary>
        private SmartBuffer[,] mCurrentTable = new SmartBuffer[cMaxTileRows, cMaxTileColumns];

        // This memory is used when making a snapshot of the current block table
        SmartBuffer[,] mSnapshot = new SmartBuffer[cMaxTileRows, cMaxTileColumns];
#endregion

#region Public methods

        /// <summary>
        /// Set a new buffer in the internal table and manage the existing one.
        /// <param name="aBuffer">New buffer to add.</param>
        /// <param name="aRow">The tiling row index of the stream.</param>
        /// <param name="aColumn">The tiling column index of the stream.</param>
        /// </summary>        
        public void Set(SmartBuffer aBuffer, int aRow, int aColumn)
        {
            if (aBuffer == null)
            {
                return;
            }
            
            lock (mBufferTableObject)
            {

                // If the element was already in the list, we do not need it anymore
                if (mCurrentTable[aRow, aColumn] != null)
                {
                    mCurrentTable[aRow, aColumn].DecreaseCount();
                }

                // Hold the new buffer and lock it into the list
                aBuffer.IncreaseCount();
                mCurrentTable[aRow, aColumn] = aBuffer;
            }
        }

        /// <summary>
        /// Get the last buffer from the snapshot
        /// The function GetBufferFromSnapshot can only be used when the snapshot is locked
        /// </summary>
        /// <param name="aRow">The tiling row index of the stream</param>
        /// <param name="aColumn">The tiling column index of the stream.</param>
        /// <returns>The buffer or null is nothing set yet</returns>
        public PvBuffer GetBufferFromSnapshot(int aRow, int aColumn)
        {
            return mSnapshot[aRow, aColumn];
        }

        /// <summary>
        /// Lock the current buffers in memory for usage by the application.
        /// </summary>
        public void LockSnapshot()
        {
            lock (mBufferTableObject)
            {
                for (int i = 0; i < cMaxTileRows; i++)
                {
                    for (int j = 0; j < cMaxTileColumns; j++)
                    {
                        // Increase the use count of the element
                        if (mCurrentTable[i, j] != null)
                        {
                            mCurrentTable[i, j].IncreaseCount();
                        }
                        mSnapshot[i, j] = mCurrentTable[i, j];
                    }
                }
            }
        }
    
        /// <summary>
        /// Unlock the current buffers in memory for usage by the application.
        /// The function GetBufferFromSnapshot can only be used when the snapshot is locked.
        /// </summary>
        public void UnlockSnapshot()
        {
            lock (mBufferTableObject)
            {
                for (int i = 0; i < cMaxTileRows; i++)
                {
                    for (int j = 0; j < cMaxTileColumns; j++)
                    {
                        // we only want to return the buffer if it has been replace in 
                        // the current table. Otherwise, we keep it for potential later usage
                        if (mSnapshot[i, j] != null)
                        {
                            mSnapshot[i, j].DecreaseCount();
                            mSnapshot[i, j] = null;
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Reset the current buffer at a given location
        /// <param name="aRow">The tiling row index of the stream.</param>
        /// <param name="aColumn">The tiling column index of the stream</param>
        /// </summary>       
        public void Reset(int aRow, int aColumn)
        {
            lock (mBufferTableObject)
            {
                if (mCurrentTable[aRow, aColumn] != null)
                {
                    mCurrentTable[aRow, aColumn].DecreaseCount();
                    mCurrentTable[aRow, aColumn] = null;
                }
            }
        }
#endregion
    }
}
