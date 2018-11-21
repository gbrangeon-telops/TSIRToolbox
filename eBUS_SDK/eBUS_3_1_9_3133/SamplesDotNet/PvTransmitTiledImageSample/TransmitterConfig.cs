// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using PvDotNet;

namespace PvTransmitTiledImageSample
{
    #region Enumerator
    public enum TilingMode
    {
        TILING_MODE_RATIO = 0,
        TILING_MODE_CROP,
        TILING_MODE_STRETCH,
    };
    #endregion

    class TransmitterConfig
    {

#region Constructor
       /// <summary>
        /// Constructor
       /// </summary>
        public TransmitterConfig()
        {
            Reset();
        }       

#endregion

#region Data members
        private UInt32 mWidth;
        private UInt32 mHeight;
        private TilingMode mMode;
        private UInt32 mFps;
        private UInt32 mPacketSize;
        private string mDestinationIPAddress;
        private UInt16 mDestinationPort;
        private string mSourceIPAddress;

#endregion

#region Properties
        public UInt32 Width
        {
            get{return mWidth;}
            set{ mWidth = value; }

        }

        public UInt32 Height
        {
            get { return mHeight; }
            set { mHeight = value; }

        }
        
        public TilingMode Mode
        {
            get { return mMode; }
            set { mMode = value; }

        }
        
        public UInt32 Fps
        {
            get { return mFps; }
            set { mFps = value; }

        }

        public UInt32 PacketSize
        {
            get { return mPacketSize; }
            set { mPacketSize = value; }

        }

        public string DestinationIPAddress
        {
            get { return mDestinationIPAddress; }
            set { mDestinationIPAddress = value; }

        }

        public UInt16 DestinationPort
        {
            get { return mDestinationPort; }
            set { mDestinationPort = value; }

        }

        public string SourceIPAddress
        {
            get { return mSourceIPAddress; }
            set { mSourceIPAddress = value; }

        }
#endregion

#region Utilities methods
      /// <summary>
        /// Reset all the field of the object
      /// </summary>
        void Reset()
        {
            mWidth = 640;
            mHeight = 480;
            mMode = TilingMode.TILING_MODE_RATIO;
            mFps = 30;
            mPacketSize = 1476;
            mDestinationIPAddress = "239.192.1.1";
            mDestinationPort = 1042;
            mSourceIPAddress = "";
        }
#endregion

#region Public methods
        public void  Store(ref PvConfigurationWriter aWriter)
        {
            try
            {
                List<PvProperty> lPropertiesList = new List<PvProperty>();
                
                lPropertiesList.Add(new PvProperty("Width", mWidth.ToString()));
                lPropertiesList.Add(new PvProperty("Height", mHeight.ToString()));
                lPropertiesList.Add(new PvProperty("Mode", ( (Int64) mMode ).ToString()));
                lPropertiesList.Add(new PvProperty("Fps", mFps.ToString()));
                lPropertiesList.Add(new PvProperty("PacketSize", mPacketSize.ToString()));
                lPropertiesList.Add(new PvProperty("DestinationIPAddress", mDestinationIPAddress));
                lPropertiesList.Add(new PvProperty("DestinationPort", mDestinationPort.ToString()));
                lPropertiesList.Add(new PvProperty("SourceIPAddress", mSourceIPAddress));

                aWriter.Store(lPropertiesList, "Transmitter Config");
            }
            catch(PvException lPvExc)
            {
                throw lPvExc;
            }

        }

        public void  Restore(ref PvConfigurationReader aReader)
        {
            List<PvProperty> lPropertiesList = new List<PvProperty>();
            string lName;
            string lTemp;

            Reset();

            try
            {
                lTemp = "Transmitter Config";
                aReader.Restore(lTemp, lPropertiesList);
                foreach (PvProperty lProperty in  lPropertiesList)
                {
                    lName = lProperty.Name;
                    if(lName == "Height")
                    {
                        mHeight = Convert.ToUInt32(lProperty.Value);
                    }
                    else if(lName == "Width")
                    {
                        mWidth = Convert.ToUInt32(lProperty.Value);
                    }
                    if(lName == "Mode")
                    {
                        mMode = (TilingMode) Convert.ToUInt32(lProperty.Value);
                    }
                    else if(lName == "Fps")
                    {
                        mFps = Convert.ToUInt32(lProperty.Value);
                    }
                    if(lName == "PacketSize")
                    {
                        mPacketSize = Convert.ToUInt32(lProperty.Value);
                    }
                    else if(lName ==  "DestinationIPAddress")
                    {
                        mDestinationIPAddress = lProperty.Value;
                    }
                    else if(lName == "DestinationPort")
                    {
                        mDestinationPort = Convert.ToUInt16(lProperty.Value);;
                    }
                    else if(lName ==  "SourceIPAddress")
                    {
                        mSourceIPAddress = lProperty.Value;
                    }
                } 

            }
            catch(PvException lPvExc)
            {
                throw lPvExc;
            }

        }
#endregion

#region
#endregion


#region
#endregion

#region
#endregion

    }
}
