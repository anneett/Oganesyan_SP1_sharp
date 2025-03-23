namespace Oganesyan_SP1_sharp
{
    partial class Form1
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.start = new System.Windows.Forms.Button();
            this.stop = new System.Windows.Forms.Button();
            this.numericUpDown = new System.Windows.Forms.NumericUpDown();
            this.listBox = new System.Windows.Forms.ListBox();
            this.send = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown)).BeginInit();
            this.SuspendLayout();
            // 
            // start
            // 
            this.start.Location = new System.Drawing.Point(441, 28);
            this.start.Name = "start";
            this.start.Size = new System.Drawing.Size(242, 22);
            this.start.TabIndex = 0;
            this.start.Text = "Start";
            this.start.UseVisualStyleBackColor = true;
            this.start.Click += new System.EventHandler(this.start_Click);
            // 
            // stop
            // 
            this.stop.Location = new System.Drawing.Point(441, 69);
            this.stop.Name = "stop";
            this.stop.Size = new System.Drawing.Size(242, 22);
            this.stop.TabIndex = 1;
            this.stop.Text = "Stop";
            this.stop.UseVisualStyleBackColor = true;
            this.stop.Click += new System.EventHandler(this.stop_Click);
            // 
            // numericUpDown
            // 
            this.numericUpDown.Location = new System.Drawing.Point(134, 29);
            this.numericUpDown.Name = "numericUpDown";
            this.numericUpDown.Size = new System.Drawing.Size(239, 22);
            this.numericUpDown.TabIndex = 2;
            // 
            // listBox
            // 
            this.listBox.FormattingEnabled = true;
            this.listBox.ItemHeight = 16;
            this.listBox.Location = new System.Drawing.Point(134, 69);
            this.listBox.Name = "listBox";
            this.listBox.Size = new System.Drawing.Size(239, 340);
            this.listBox.TabIndex = 3;
            // 
            // send
            // 
            this.send.Location = new System.Drawing.Point(441, 112);
            this.send.Name = "send";
            this.send.Size = new System.Drawing.Size(242, 22);
            this.send.TabIndex = 4;
            this.send.Text = "Send";
            this.send.UseVisualStyleBackColor = true;
            this.send.Click += new System.EventHandler(this.send_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(441, 159);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(242, 250);
            this.textBox1.TabIndex = 7;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.send);
            this.Controls.Add(this.listBox);
            this.Controls.Add(this.numericUpDown);
            this.Controls.Add(this.stop);
            this.Controls.Add(this.start);
            this.Name = "Form1";
            this.Text = "Oganesyan";
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button start;
        private System.Windows.Forms.Button stop;
        private System.Windows.Forms.NumericUpDown numericUpDown;
        private System.Windows.Forms.ListBox listBox;
        private System.Windows.Forms.Button send;
        private System.Windows.Forms.TextBox textBox1;
    }
}

