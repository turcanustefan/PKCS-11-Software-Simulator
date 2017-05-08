namespace GenerareFormular
{
    partial class Formular
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.locationCount = new System.Windows.Forms.ComboBox();
            this.valueCount = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.typeCount = new System.Windows.Forms.ComboBox();
            this.formGenerateButton = new System.Windows.Forms.Button();
            this.formCount = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // locationCount
            // 
            this.locationCount.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.locationCount.FormattingEnabled = true;
            this.locationCount.Location = new System.Drawing.Point(159, 118);
            this.locationCount.Name = "locationCount";
            this.locationCount.Size = new System.Drawing.Size(121, 24);
            this.locationCount.TabIndex = 2;
            // 
            // valueCount
            // 
            this.valueCount.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.valueCount.FormattingEnabled = true;
            this.valueCount.Location = new System.Drawing.Point(471, 118);
            this.valueCount.Name = "valueCount";
            this.valueCount.Size = new System.Drawing.Size(121, 24);
            this.valueCount.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(8, 118);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(143, 17);
            this.label1.TabIndex = 1;
            this.label1.Text = "Number of Locations:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(340, 121);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(125, 17);
            this.label2.TabIndex = 2;
            this.label2.Text = "Number of Values:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(343, 67);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(80, 17);
            this.label3.TabIndex = 1;
            this.label3.Text = "Form Type:";
            // 
            // typeCount
            // 
            this.typeCount.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.typeCount.FormattingEnabled = true;
            this.typeCount.Location = new System.Drawing.Point(471, 64);
            this.typeCount.Name = "typeCount";
            this.typeCount.Size = new System.Drawing.Size(121, 24);
            this.typeCount.TabIndex = 1;
            // 
            // formGenerateButton
            // 
            this.formGenerateButton.Location = new System.Drawing.Point(261, 202);
            this.formGenerateButton.Name = "formGenerateButton";
            this.formGenerateButton.Size = new System.Drawing.Size(162, 45);
            this.formGenerateButton.TabIndex = 4;
            this.formGenerateButton.Text = "Generate Forms";
            this.formGenerateButton.UseVisualStyleBackColor = true;
            this.formGenerateButton.Click += new System.EventHandler(this.formGenerateButton_Click);
            // 
            // formCount
            // 
            this.formCount.Location = new System.Drawing.Point(159, 67);
            this.formCount.Name = "formCount";
            this.formCount.Size = new System.Drawing.Size(121, 22);
            this.formCount.TabIndex = 0;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(8, 67);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(121, 17);
            this.label4.TabIndex = 1;
            this.label4.Text = "Number of Forms:";
            // 
            // Formular
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(665, 291);
            this.Controls.Add(this.formCount);
            this.Controls.Add(this.formGenerateButton);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.typeCount);
            this.Controls.Add(this.valueCount);
            this.Controls.Add(this.locationCount);
            this.MaximizeBox = false;
            this.Name = "Formular";
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox locationCount;
        private System.Windows.Forms.ComboBox valueCount;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox typeCount;
        private System.Windows.Forms.Button formGenerateButton;
        private System.Windows.Forms.TextBox formCount;
        private System.Windows.Forms.Label label4;
    }
}

