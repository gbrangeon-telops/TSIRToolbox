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

namespace TransmitTiledImages
{
    /// <summary>
    /// Communication setup data holder.
    /// </summary>
    public class Setup
    {
#region Constants
        // Current possible role of a device
        public const int cRoleInvalid  = 0;
        public const int cRoleCtrlData = 1;
        public const int cRoleData     = 2;

        // Current possible destination 
        public const int cDestinationInvalid         = 0;
        public const int cDestinationUnicastAuto     = 1;
        public const int cDestinationUnicastSpecific = 2;
        public const int cDestinationMulticast       = 3;
#endregion

#region Constructor
        public Setup()
        {
            Reset();
        }
#endregion

#region Data members
        // Role of the connection
        UInt16 mRole;

        // Destination 
        UInt16 mDestination;

        // Optional IP address
        string mIPAddress;
        
        // Optional Port
        UInt16 mPort;
#endregion

#region Properties
        public UInt16 Role
        {
            get { return mRole; }
            set { mRole = value; }
        }

        public UInt16 Destination
        {
            get { return mDestination; }
            set { mDestination = value; }
        }

        public string IPAddress
        {
            get { return mIPAddress; }
            set { mIPAddress = value; }
        }
        public UInt16 Port
        {
            get { return mPort; }
            set { mPort = value; }
        }
#endregion

#region public methods
        /// <summary>
        /// Reset all the internal informations.
        /// </summary>
        public void Reset()
        {
            mRole = cRoleInvalid;
            mDestination = cDestinationInvalid;
            mIPAddress = ("");
            mPort = 0;
        }
    
        /// <summary>
        /// Store the class into a persistence object.
        /// </summary>
        /// <param name="aWriter">The PvConfigurationWriter object</param>
        /// <param name="aRow">The tiling index row</param>
        /// <param name="aColumn">The tiling index column</param>
        public void Store(ref PvConfigurationWriter aWriter, int aRow, int aColumn)
        {
            string lTemp;           
            List <PvProperty> lPropertyList = new List<PvProperty>();

            PvProperty lProperty = new PvProperty("Role", mRole.ToString());
            lPropertyList.Add(lProperty);

            lProperty = new PvProperty("Destination", mDestination.ToString());
            lPropertyList.Add(lProperty);

            lProperty = new PvProperty("IPAddress", IPAddress.ToString());
            lPropertyList.Add(lProperty);

            lProperty = new PvProperty("Port", mPort.ToString());
            lPropertyList.Add(lProperty);

            lTemp = string.Format("Setup( {0}, {1} )", aRow, aColumn);
            try
            {
                aWriter.Store(lPropertyList, lTemp);
            }
            catch (PvException lPvExc)
            {
                throw lPvExc;
            }
        }

        /// <summary>
        /// Load the class from a persistence object.
        /// </summary>
        /// <param name="aReader">The PvConfigurationReader object.</param>
        /// <param name="aRow">The tiling index row.</param>
        /// <param name="aColumn">The tiling index column.</param>
        public void Restore(ref PvConfigurationReader aReader, int aRow, int aColumn)
        {
            List<PvProperty> lPvPropertyList = new List<PvProperty>();
            string lName;
            string lTemp;

            Reset();

            lTemp = string.Format("Setup( {0}, {1} )", aRow, aColumn);
            aReader.Restore(lTemp, lPvPropertyList);

            for (int i = 0; i < lPvPropertyList.Count; i++)
            {
                lName = lPvPropertyList[i].Name;
                if (lName == "Role")
                {
                    mRole = Convert.ToUInt16(lPvPropertyList[i].Value);
                }
                else if (lName == "Destination")
                {
                    mDestination = Convert.ToUInt16(lPvPropertyList[i].Value);
                }
                else if (lName == "IPAddress")
                {
                    mIPAddress = lPvPropertyList[i].Value;
                }
                else if (lName == "Port")
                {
                    mPort = Convert.ToUInt16(lPvPropertyList[i].Value);
                }
            }
        }
#endregion
    }
}
