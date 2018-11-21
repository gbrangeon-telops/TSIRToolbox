// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Drawing;
using PvDotNet;


namespace TransmitTiledImages
{
    /// <summary>
    /// Holds a PvBuffer and the Bitmap it is attached to
    /// </summary>
    public class TransmitItem
    {
        public TransmitItem(int aIndex, int aWidth, int aHeight)
        {
            mBuffer = new PvBuffer();
            mBuffer.ID = (ulong)aIndex;

            mBitmap = new Bitmap(aWidth, aHeight, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
        }

        public PvBuffer Buffer { get { return mBuffer; } }
        public Bitmap Bitmap { get { return mBitmap; } }

        /// <summary>
        /// Attach bitmap data to PvBuffer
        /// </summary>
        unsafe public void Attach()
        {
            if (mBitmapData != null)
            {
                Debug.Fail("Unexecpected case");
                return;
            }

            // Lock bitmap data
            mBitmapData = mBitmap.LockBits(new Rectangle(0, 0, mBitmap.Width, mBitmap.Height),
                System.Drawing.Imaging.ImageLockMode.ReadOnly, mBitmap.PixelFormat);
            Debug.Assert(mBitmapData != null);

            // Attach to PvBuffer
            mBuffer.Image.Attach((byte*)mBitmapData.Scan0,
                (uint)mBitmap.Width, (uint)mBitmap.Height, PvPixelType.BGR8);
        }

        /// <summary>
        /// Detach bitmap data from PvBuffer
        /// </summary>
        unsafe public void Detach()
        {
            if (mBitmapData == null)
            {
                Debug.Fail("Unexpected case");
                return;
            }

            // Detach from PvBuffer
            mBuffer.Detach();

            // Unlock bitmap data
            mBitmap.UnlockBits(mBitmapData);
            mBitmapData = null;
        }

        private PvBuffer mBuffer = null;

        private Bitmap mBitmap = null;
        private System.Drawing.Imaging.BitmapData mBitmapData = null;
    }
}

