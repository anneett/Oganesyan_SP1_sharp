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
        Process childProcess;

        [DllImport(@"C:\Users\egiaz\Documents\GitHub\Oganesyan_SP1\Oganesyan_SP1_sharp\x64\Debug\Oganesyan_Dll.dll",
            CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        private static extern void startThread(int selected_thread, string text);

        [DllImport(@"C:\Users\egiaz\Documents\GitHub\Oganesyan_SP1\Oganesyan_SP1_sharp\x64\Debug\Oganesyan_Dll.dll",
            CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        private static extern void stopThread(int selected_thread, string text);

        [DllImport(@"C:\Users\egiaz\Documents\GitHub\Oganesyan_SP1\Oganesyan_SP1_sharp\x64\Debug\Oganesyan_Dll.dll",
            CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        private static extern void sendData(int selected_thread, string text);

        [DllImport(@"C:\Users\egiaz\Documents\GitHub\Oganesyan_SP1\Oganesyan_SP1_sharp\x64\Debug\Oganesyan_Dll.dll",
            CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        private static extern void confirmThread(int selected_thread, string text);
        public Form1()
        {
            InitializeComponent();
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
            }
            else
            {
                int existingThreads = listBox.Items.Count - 2;
                for (int i = 0; i < N; i++)
                {
                    startThread(existingThreads + i, "StartThread");
                    confirmThread(existingThreads + i, "Confirm");
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
                stopThread(0, "Close");
                confirmThread(0, "Confirm");
                childProcess = null;
            }
            else
            {
                stopThread(listBox.Items.Count - 3, "StopThread");
                confirmThread(listBox.Items.Count - 3, "Confirm");
                listBox.Items.RemoveAt(listBox.Items.Count - 1);

                if (listBox.Items.Count <= 2)
                {
                    stopThread(0, "Close");
                    confirmThread(0, "Confirm");
                    childProcess = null;
                }
            }
        }
        private void send_Click(object sender, EventArgs e)
        {
            if (!string.IsNullOrEmpty(textBox1.Text) && listBox.SelectedItem != null)
            {
                int selectedThread = -2;
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
             
                sendData(selectedThread, textBox1.Text);
                confirmThread(selectedThread, "Confirm");
            }
        }
    }
}
