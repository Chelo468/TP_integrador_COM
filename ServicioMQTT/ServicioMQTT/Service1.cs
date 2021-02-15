using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration;
using System.Data;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading.Tasks;
using System.Timers;

namespace ServicioMQTT
{
    public partial class Service1 : ServiceBase
    {
        private Timer _TimerMain = new Timer();

        private double _TimeInterval { get; set; }
        private string _SqlConnection { get; set; }
        private string _logPath { get; set; }
        public Service1()
        {
            InitializeComponent();

            System.Net.ServicePointManager.SecurityProtocol = (System.Net.SecurityProtocolType)3072;
            System.Net.ServicePointManager.ServerCertificateValidationCallback = ((sender, certificate, chain, sslPolicyErrors) => true);

            _TimeInterval = double.Parse(ConfigurationManager.AppSettings["TimeInterval"]);
            _SqlConnection = ConfigurationManager.AppSettings["sqlConnection"];

            _TimerMain.Interval = _TimeInterval;
            _TimerMain.Elapsed += TimerMain_Elapsed;

            _TimerMain.Enabled = true;
        }

        protected override void OnStart(string[] args)
        {
            try
            {
                ConectorMQTT.Instancia().conectar();
                //Log("Arranca el servicio");

                //_TimerMain.Start();
            }
            catch (Exception ex)
            {
                Log("Error 1000 al enviar archivo a la base de datos. A continaución se enviara mensaje de exception.");
                Log(ex.Message);
                Log(ex.Source);

                return;
            }
        }

        protected override void OnStop()
        {
            _TimerMain.Stop();
        }

        void TimerMain_Elapsed(object sender, ElapsedEventArgs e)
        {
            try
            {
                Procesar();
            }
            catch (Exception ex)
            {
                Log("Error 1003 procesando los archivos. A continuación se enviara mensaje de exception.");
                Log(ex.Message);
                Log(ex.Source);
            }
            
        }

        private void Procesar()
        {
            _TimerMain.Stop();
        }

        /// <summary>
        /// Log de eventos y errores del sistema en archivo txt
        /// </summary>
        /// <param name="pLogMessage">mensaje de error</param>
        public void Log(String pLogMessage)
        {
            try
            {
                using (StreamWriter debugFile = File.AppendText(Path.Combine(_logPath, "debug.txt")))
                {
                    debugFile.Write("\r\nEvento : ");
                    debugFile.WriteLine("{0} {1}", DateTime.Now.ToLongTimeString(), DateTime.Now.ToLongDateString());
                    debugFile.WriteLine("  >>{0}", pLogMessage);
                    debugFile.WriteLine("-------------------------------");

                    debugFile.Flush();
                    debugFile.Close();
                };

            }
            catch (Exception ex)
            {
                //TODO: manejar esto de alguna forma.
            }
        }
    }
}
