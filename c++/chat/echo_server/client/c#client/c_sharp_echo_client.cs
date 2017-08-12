using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Diagnostics;
using System.Net.Sockets;
using System.IO;

namespace WpfApp1
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            Debug.WriteLine("Hi");
            string addr = "192.168.0.46";

            Int32 port = 27015;

            TcpClient client = new TcpClient(addr, port);

            

            Stream stream = client.GetStream();

            StreamWriter writer = new StreamWriter(stream);

            Byte[] data = System.Text.Encoding.ASCII.GetBytes("Hi");

            stream.Write(data, 0, data.Length);



            data = new Byte[256];

            String responseData = String.Empty;

            Int32 bytes = stream.Read(data, 0, data.Length);
            responseData = System.Text.Encoding.ASCII.GetString(data, 0, bytes);
            Console.WriteLine("Received: {0}", responseData);
            Debug.WriteLine("received :  " + responseData);
            writer.WriteLine("Testing...");

            stream.Close();

            client.Close();
        }
    }
}
