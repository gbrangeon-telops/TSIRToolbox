using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;

namespace ImageProcessing
{
    class ColorMatrix
    {
       
        public ColorMatrix( )
        {
           
        }

        public float[][] Matrix { get; set; }

        public void Apply(Bitmap aOriginalImage)
        {
            Graphics lGraphics = Graphics.FromImage(aOriginalImage);
            System.Drawing.Imaging.ColorMatrix lColorMatrix = new System.Drawing.Imaging.ColorMatrix(Matrix);
            ImageAttributes lImageAttributes = new ImageAttributes();
            lImageAttributes.SetColorMatrix(lColorMatrix);
            lGraphics.DrawImage(aOriginalImage, new Rectangle(0, 0, aOriginalImage.Width, aOriginalImage.Height),
                0, 0, aOriginalImage.Width, aOriginalImage.Height,
                GraphicsUnit.Pixel,
                lImageAttributes);
          
        }
    }
}
