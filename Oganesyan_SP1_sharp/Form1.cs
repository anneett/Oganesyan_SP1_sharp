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
            CharSet = CharSet.Unicode)]
        private static extern void connect(int selected_thread, string text);

        [DllImport(@"C:\Users\egiaz\Documents\GitHub\Oganesyan_SP1\Oganesyan_SP1_sharp\x64\Debug\Oganesyan_Dll.dll",
            CharSet = CharSet.Unicode)]
        private static extern void disconnect(int selected_thread, string text);

        [DllImport(@"C:\Users\egiaz\Documents\GitHub\Oganesyan_SP1\Oganesyan_SP1_sharp\x64\Debug\Oganesyan_Dll.dll",
            CharSet = CharSet.Unicode)]
        private static extern void sendCommand(int selected_thread, int commandId, string message);

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
            int existingThreads = listBox.Items.Count - 2;

            for (int i = 0; i < N; i++)
            {
                sendCommand(existingThreads + i, 0, "Создать поток");
                listBox.Items.Add($"Поток {existingThreads + i}");
            }
        }
        private void stop_Click(object sender, EventArgs e)
        {

            if (listBox.Items.Count <= 2)
            {
                sendCommand(-1, 1, "Завершить все");
            }
            else
            {
                int threadId = listBox.Items.Count - 3;
                sendCommand(threadId, 1, "Завершить поток");
                listBox.Items.RemoveAt(listBox.Items.Count - 1);
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

                sendCommand(selectedThread, 3, textBox1.Text);
            }
        }

    }
}
