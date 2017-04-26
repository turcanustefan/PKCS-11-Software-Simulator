using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using ZXing;
using ZXing.QrCode.Internal;
using ZXing.Rendering;
using ZXing.QrCode;
using System.Drawing.Printing;
using System.Drawing.Imaging;

namespace GenerareFormular
{
    public partial class Formular : Form
    {
        public Formular()
        {
            InitializeComponent();
            Initialise();
        }
        static int FormCounter=0;
        int height = 1200;
        int width = 1800;
        int fontSize = 14;
        int qrSize = 120;
        string fontType = "Times New Roman";
        int circleSize = 30;
        int circlePen = 1;
        int circleXDist = 10;
        int circleYDist = 15;
        int rectDist = 20;
        int circlePerRow = 10;
        int marginHeight = 30;
        int valueTextY = 50;
        int locationTextY = 50;
        int valueRectSizeX;
        int valueRectSizeY;
        int typeTextX;
        int typeTextY;
        int locationRectSizeX;
        int locationRectSizeY;

        // Initialise ---------------------------------------------------------
        public void Initialise()
        {
            for (int i=1;i<=4;i++)
                typeCount.Items.Add(i.ToString());
            for (int i = 1; i <= 10; i++)
                locationCount.Items.Add(i);
            for (int i = 10; i <= 100; i+=10)
                valueCount.Items.Add(i);

            typeCount.SelectedIndex = 0;
            locationCount.SelectedIndex = 0;
            valueCount.SelectedIndex = 0;

        }
        // Printing -----------------------------------------------------------

        // Drawing ------------------------------------------------------------
        public void DrawString(Bitmap bmp, float x, float y, string drawString, string fontType, int size)
        {
            System.Drawing.Graphics formGraphics = Graphics.FromImage(bmp);
            System.Drawing.Font drawFont = new System.Drawing.Font(fontType, size, FontStyle.Regular);
            System.Drawing.SolidBrush drawBrush = new System.Drawing.SolidBrush(System.Drawing.Color.Black);
            System.Drawing.StringFormat drawFormat = new System.Drawing.StringFormat();
            formGraphics.DrawString(drawString, drawFont, drawBrush, x, y, drawFormat);
            drawFont.Dispose();
            drawBrush.Dispose();
            formGraphics.Dispose();
        }
        public void DrawRect(Bitmap bmp, float x, float y, float width, float height, int borderWidth)
        {
            Pen pen = new Pen(Color.Black, borderWidth);
            System.Drawing.Graphics formGraphics = Graphics.FromImage(bmp);
            formGraphics.DrawRectangle(pen, x, y, width, height);
            formGraphics.Dispose();
        }
        public void DrawCircle(Bitmap bmp, float x, float y, int size, int circleSize)
        {
            Pen pen = new Pen(Color.Black, size);
            System.Drawing.Graphics formGraphics = Graphics.FromImage(bmp);
            formGraphics.DrawEllipse(pen, x, y, circleSize, circleSize);
            formGraphics.Dispose();
        }
        public void DrawForms(Bitmap bmp, int x_coord, int y_coord, string qrText, int locationCircles, int valueCircles)
        {
            int initX = x_coord;
            int initY = y_coord;

            // QR
            Graphics g = Graphics.FromImage(bmp);
            Point xy = new Point(x_coord - qrSize , y_coord - qrSize + 40);
            g.DrawImage(GenerateQR(qrSize, qrText), xy);

            // Valoare inregistrare
            DrawRect(bmp, x_coord, y_coord - 60, valueRectSizeX, valueTextY, 1);
            DrawString(bmp, x_coord + 70, y_coord - 45, "VALOARE ÎNREGISTRARE", fontType, 14);
            DrawRect(bmp, x_coord, y_coord, valueRectSizeX, valueRectSizeY, 2);
            int nr = 1;
            for (int i = 0; i < valueCircles/10; i++)
            {
                for (int j = 0; j < circlePerRow; j++)
                {
                    DrawCircle(bmp, x_coord + 5, y_coord + circleYDist, circlePen, circleSize);
                    if (nr < 10)
                        DrawString(bmp, x_coord + 12, y_coord + 20, nr.ToString(), fontType, fontSize);
                    else
                    {
                        if (nr < 100)
                            DrawString(bmp, x_coord + 8, y_coord + 20, nr.ToString(), fontType, fontSize);
                        else
                            DrawString(bmp, x_coord + 2, y_coord + 20, nr.ToString(), fontType, fontSize);
                    }
                    nr++;
                    x_coord += circleXDist + circleSize;
                }
                x_coord -= (circleXDist + circleSize)*circlePerRow;
                y_coord += 45;
            }
            y_coord -= 45 * valueCircles / 10;
            int x = x_coord - valueRectSizeX - 20, y = y_coord + 10 + typeTextY;
            // Locatie Text + Box
            DrawRect(bmp, x, y - 60, locationRectSizeX, locationTextY, 1);
            DrawString(bmp, x + 120, y - 45, "NUMĂR LOCAȚIE", fontType, 14);
            // Locatie Value Box
            DrawRect(bmp, x, y, locationRectSizeX, locationRectSizeY, 2);
            // Tip Formular
            DrawRect(bmp, x, y - 150, typeTextX, typeTextY, 1);
            DrawString(bmp, x + 75, y - 120, "TIP FORMULAR", fontType, 14);

            for (int i = 0; i < locationCircles; i++)
            {
                for (int j = 0; j < circlePerRow; j++)
                {
                    DrawCircle(bmp, x + 5, y + 15, circlePen, circleSize);
                    DrawString(bmp, x + 12, y + 19, j.ToString(), fontType, fontSize);
                    x += circleXDist + circleSize;
                }
                x -= (circleXDist + circleSize) * circlePerRow;
                y += (circleYDist + circleSize);
            }
        }
        public Bitmap GenerateQR(int qrSize, string qrText)
        {
            BarcodeWriter writer = new BarcodeWriter { Format = BarcodeFormat.QR_CODE ,
                Options = new QrCodeEncodingOptions
                {
                    Width = qrSize ,
                    Height = qrSize
                }
            };
            var result = writer.Write(qrText);
            var barcodeBitmap = new Bitmap(result);
            return barcodeBitmap;
        }

        private void formGenerateButton_Click(object sender, EventArgs e)
        {
            //int noOfForms = Convert.ToInt32(formCount.SelectedText);
            int type = Convert.ToInt32(typeCount.Text);
            int location = Convert.ToInt32(locationCount.Text);
            int value = Convert.ToInt32(valueCount.Text);
            // Calculam valorile necesare
            locationRectSizeX = circleXDist * (circlePerRow) + (circlePerRow * circleSize);
            locationRectSizeY = circleYDist * (location + 1) + (location * circleSize);

            valueRectSizeX = circleXDist * (circlePerRow) + (circlePerRow * circleSize);
            valueRectSizeY = circleYDist * (value / circlePerRow + 1) + (value / circlePerRow * circleSize);

            int locationTextX = locationRectSizeX;
            int valueTextX = valueRectSizeX;
            typeTextX = valueRectSizeX - qrSize + 16;
            typeTextY = 80;

            width = (marginHeight * 2 + locationRectSizeX + valueRectSizeX + rectDist*2) * 2;
            height = (marginHeight * 2 + Math.Max(valueTextY + valueRectSizeY + 10, 20 + typeTextY + locationTextY + locationRectSizeY) + rectDist) * 2;

            int x_coord = 460;
            int y_coord = 100;
            int x_dim = width / 2;
            int y_dim = height / 2;

            Image resultImage = new Bitmap(width, height, PixelFormat.Format24bppRgb);
            using (Graphics grp = Graphics.FromImage(resultImage))
            {
                grp.FillRectangle(Brushes.White, 0, 0, width, height);
            }
            Bitmap bmp;
            bmp = new Bitmap(resultImage);

            // Draw Forms
            DrawForms(bmp, x_coord, y_coord, type.ToString() + '-' + FormCounter.ToString(), location, value);
            FormCounter++;
            DrawForms(bmp, x_coord + x_dim, y_coord, type.ToString() + '-' + FormCounter.ToString(), location, value);
            FormCounter++;
            DrawForms(bmp, x_coord, y_coord + y_dim, type.ToString() + '-' + FormCounter.ToString(), location, value);
            FormCounter++;
            DrawForms(bmp, x_coord + x_dim, y_coord + y_dim, type.ToString() + '-' + FormCounter.ToString(), location, value);
            FormCounter++;

            // Draw Middle Lines
            Pen pen = new Pen(Color.Black, 1);
            System.Drawing.Graphics formGraphics = Graphics.FromImage(bmp);
            formGraphics.DrawLine(pen, x_dim, 0, x_dim, height);
            formGraphics.DrawLine(pen, 0, y_dim, width, y_dim);
            formGraphics.DrawLine(pen, 0, 0, width, 0);
            formGraphics.DrawLine(pen, 0, height - 1, width, height - 1);
            formGraphics.DrawLine(pen, 0, 0, 0, height);
            formGraphics.DrawLine(pen, width - 1, 0, width - 1, height);
            formGraphics.Dispose();

            bmp.Save("formular_complet.jpg");
            bmp.Dispose();
        }
        // ---------------------------------------------------------------------
    }
}
