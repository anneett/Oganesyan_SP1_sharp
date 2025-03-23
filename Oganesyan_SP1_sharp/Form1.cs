using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Reflection.Emit;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.LinkLabel;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace Oganesyan_SP1_sharp
{
    public partial class Form1 : Form
    {
        Process childProcess = null;
        System.Threading.EventWaitHandle stopEvent = new EventWaitHandle(false, EventResetMode.ManualReset, "StopEvent");
        System.Threading.EventWaitHandle startEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "StartEvent");
        System.Threading.EventWaitHandle closeEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "CloseEvent");
        System.Threading.EventWaitHandle sendEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "SendEvent");
        System.Threading.EventWaitHandle confirmEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "ConfirmEvent");

        [DllImport(@"C:\Users\egiaz\Documents\GitHub\Oganesyan_SP1\Oganesyan_SP1_sharp\x64\Debug\Oganesyan_Dll.dll", CharSet = CharSet.Unicode)]
        private static extern void SendData(int selected_thread, string text);
        public Form1()
        {
            InitializeComponent();
            this.FormClosing += Form_Closing;
        }
        private void Form_Closing(object sender, FormClosingEventArgs e)
        {
            if (childProcess != null && !childProcess.HasExited)
            {
                closeEvent.Set();
                childProcess = null;
            }
        }
        private void ChildProcess_Exited(object sender, EventArgs e)
        {
            if (InvokeRequired)
            {
                this.Invoke(new Action(() =>
                {
                    listBox.Items.Clear();
                    childProcess = null;
                }));
            }
            else
            {
                listBox.Items.Clear();
                childProcess = null;
            }
        }
        private void start_Click(object sender, EventArgs e)
        {

            if (listBox.Items.Count == 0)
            {
                listBox.Items.Add("Все потоки");
                listBox.Items.Add("Главный поток");
            }

            int N = (int)numericUpDown.Value;

            if (childProcess == null || childProcess.HasExited)
            {
                childProcess = Process.Start("C:/Users/egiaz/Documents/GitHub/Oganesyan_SP1/Oganesyan_SP1_sharp/x64/Debug/Oganesyan_SP1_plus.exe");
                childProcess.EnableRaisingEvents = true;
                childProcess.Exited += ChildProcess_Exited;
            }
            else
            {
                int existingThreads = listBox.Items.Count - 2;
                for (int i = 0; i < N; i++)
                {
                    startEvent.Set();
                    confirmEvent.WaitOne();
                    listBox.Items.Add($"Поток {existingThreads + i}");
                }
            }
        }
        private void stop_Click(object sender, EventArgs e)
        {
            if (childProcess == null || childProcess.HasExited)
            {
                return;
            }

            if (listBox.Items.Count <= 2)
            {
                closeEvent.Set();
                childProcess = null;
            }
            else
            {
                stopEvent.Set();
                confirmEvent.WaitOne();
                listBox.Items.RemoveAt(listBox.Items.Count - 1);

                if (listBox.Items.Count <= 2)
                {
                    closeEvent.Set();
                    childProcess = null;
                }
            }
        }
        private void send_Click(object sender, EventArgs e)
        {
            if (!string.IsNullOrEmpty(textBox1.Text))
            {
                int selectedThread = -2;
                if (listBox.SelectedItem != null)
                {
                    string sel = listBox.SelectedItem.ToString();
                    if (sel == "Главный поток")
                        selectedThread = -1;
                    else if (sel != "Все потоки")
                    {
                        string[] parts = sel.Split(' ');
                        if (parts.Length == 2 && int.TryParse(parts[1], out int threadNum))
                        {
                            selectedThread = threadNum;
                        }
                    }
                }

                SendData(selectedThread, textBox1.Text);
                sendEvent.Set();
                confirmEvent.WaitOne();
            }
        }
    }
}
