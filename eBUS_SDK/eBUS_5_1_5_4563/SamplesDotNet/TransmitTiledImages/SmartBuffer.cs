// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using PvDotNet;

namespace TransmitTiledImages
{
    /// <summary>
    /// PvBuffer that can be used as a smart object with a reference count.
    /// The memory manipulation is self contains.
    /// </summary>
    class SmartBuffer : PvBuffer
    {
        public SmartBuffer(ref Queue<SmartBuffer> aReturnQueue) : base(PvPayloadType.Image)                            
        {
            mReturnQueue = aReturnQueue;
            mUseCount = 0;
        }

        /// <summary>
        /// Queue to return the buffer to the stream without blocking the acquisition path.
        /// </summary>
        private Queue<SmartBuffer> mReturnQueue;

        /// <summary>
        /// Keep track of the number of user to be able to release the buffer to the stream only when needed.
        /// </summary>
        int mUseCount;

        // Object to be used to protect the use count when its value has been changed.
        private Object mSmartBufferObject = new Object();

        /// <summary>
        /// Increase the internal use count of the buffer
        /// </summary>
        public void IncreaseCount()
        {
            lock(mSmartBufferObject)
            {
                mUseCount++;
            }
        }
      
        /// <summary>
        /// Decrease the internal use count of the buffer.
        /// If no more user, the buffer is returned to the stream via the return queue.
        /// </summary>
        public void DecreaseCount()
        {
            lock(mSmartBufferObject)
            {
                if((--mUseCount) == 0)
                {
                    mReturnQueue.Enqueue( this );
                }
            }
        }
    }
}
