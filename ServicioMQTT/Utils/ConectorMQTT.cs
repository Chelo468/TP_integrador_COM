using DataAccess;
using Entidades;
using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Web.UI;
using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Messages;

namespace Utils
{
    public class ConectorMQTT
    {
        private static ConectorMQTT _instancia;
        private MqttClient clienteMqtt;
        private string clientId;
        private IUpdatable pagina;
        private Page page;

        string TOPIC_PETICION_CONFIGURACION = "/com/cuna/getConfiguration";
        string TOPIC_GET_CONFIGURACION = "/com/cuna/myConfiguration";


        public static ConectorMQTT Instancia()
        {
            if (_instancia == null)
            {
                _instancia = new ConectorMQTT();


            }
            return _instancia;
        }

        private ConectorMQTT()
        {

        }

        public void conectar(string broker)
        {
            if(clienteMqtt == null || !clienteMqtt.IsConnected)
            {
                clienteMqtt = new MqttClient(broker);

                clienteMqtt.MqttMsgPublishReceived += client_MqttMsgPublishReceived;

                clientId = "ServicioMQTT-" + Guid.NewGuid().ToString();

                clienteMqtt.Connect(clientId);
            }
            
        }

        public bool suscribir(string topic)
        {
            try
            {
                if(clienteMqtt != null && !clienteMqtt.IsConnected)
                {
                    clienteMqtt.Connect(clientId);
                }

                clienteMqtt.Subscribe(new string[] { topic }, new byte[] { 2 });
            }
            catch (Exception)
            {
                return false;
            }

            return true;
        }

        public void publicar(string topico, string mensaje)
        {
            if (!clienteMqtt.IsConnected)
            {
                clienteMqtt.Connect(clientId);
            }

            clienteMqtt.Publish(topico, Encoding.UTF8.GetBytes(mensaje), MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, true);
        }

        public void setPagina(IUpdatable pagina, ref Page page)
        {
            this.pagina = pagina;
            this.page = page;
        }

        public ConfiguracionESP32 getConfiguracion(bool enviarRequest)
        {
            suscribir(TOPIC_GET_CONFIGURACION);

            if(enviarRequest)
            {
                publicar(TOPIC_PETICION_CONFIGURACION, "");
            }
            

            DateTime momentoConsulta = DateTime.Now;

            Thread.Sleep(1500);

            ConfiguracionESP32 configuracion = MensajeDataAccess.getConfiguration(momentoConsulta);

            if(enviarRequest)
            {
                publicar(TOPIC_PETICION_CONFIGURACION, "");
            }
            

            return configuracion;
        }

        void client_MqttMsgPublishReceived(object sender, MqttMsgPublishEventArgs e)
        {
            DateTime momento = DateTime.Now;

            string mensajeRecibido = Encoding.UTF8.GetString(e.Message);
                        
            guardarEnDB(e.Topic, mensajeRecibido, momento);

            //this.pagina.actualizar(ref page);
            //this.contexto.Response.Redirect("~/Configuracion", false);
        }

        private void guardarEnDB(string topic, string mensaje, DateTime fecha)
        {
            MensajeDataAccess.MensajeInsert(topic, mensaje, fecha);
        }
    }
}
