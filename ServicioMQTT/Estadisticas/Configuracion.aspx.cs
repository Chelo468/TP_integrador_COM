using Entidades;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using Utils;

namespace Estadisticas
{
    public partial class Configuracion : System.Web.UI.Page, IUpdatable
    {
        bool alarmaTemperaturaActivada
        {
            get
            {
                return chkAlarmaTemperatura.Checked;
            }
        }

        bool alarmaSonidoActivada
        {
            get
            {
                return chkAlarmaSonido.Checked;
            }
        }

        bool alarmaProximidadActivada
        {
            get
            {
                return chkAlarmaProximidad.Checked;
            }
        }

        string temperaturaMinima
        {
            get
            {
                return txtTemperaturaMinima.Text;
            }
        }

        string temperaturaMaxima
        {
            get
            {
                return txtTemperaturaMaxima.Text;
            }
        }

        string TOPIC_ACTIVAR_SONIDO = "/com/cuna/switchAlarmaSonido";
        
        string TOPIC_ACTIVAR_PROXIMIDAD = "/com/cuna/switchAlarmaProximidad";

        string TOPIC_ACTIVAR_TEMPERATURA = "/com/cuna/switchAlarmaTemperatura";
        string TOPIC_TEMPERATURA_MINIMA = "/com/cuna/configTemperaturaMinima";
        string TOPIC_TEMPERATURA_MAXIMA = "/com/cuna/configTemperaturaMaxima";

        protected void Page_Load(object sender, EventArgs e)
        {
            if(!IsPostBack)
            {
                obtenerConfiguracionESP32(true);
            }
            
        }

        private void obtenerConfiguracionESP32(bool enviarRequest)
        {
            ConectorMQTT.Instancia().conectar(ConfigurationManager.AppSettings["brokerMqtt"]);

            var page = this.Page;

            ConectorMQTT.Instancia().setPagina(this,ref page);

            ConfiguracionESP32 configuracion = ConectorMQTT.Instancia().getConfiguracion(enviarRequest);

            configurarFormulario(configuracion);
        }

        public void actualizar(ref Page page)
        {
            obtenerConfiguracionESP32(false);

            page.ClientScript.RegisterClientScriptBlock(this.GetType(),"actualizar", "<script>console.log('Hola Mundo');</script>");//__doPostBack('')

        }

        private void configurarFormulario(ConfiguracionESP32 configuracion)
        {
            bool alarmaTemperaturaActivada = configuracion.esAlarmaTemperaturaActivada();
            
            chkAlarmaTemperatura.Checked = alarmaTemperaturaActivada;
            txtTemperaturaMaxima.Enabled = alarmaTemperaturaActivada;
            txtTemperaturaMinima.Enabled = alarmaTemperaturaActivada;
            btnSetearTemperaturas.Enabled = alarmaTemperaturaActivada;

            if(alarmaTemperaturaActivada)
            {
                txtTemperaturaMinima.Text = configuracion.temperatura_minima;
                txtTemperaturaMaxima.Text = configuracion.temperatura_maxima;
            }

            bool alarmaSonidoActivada = configuracion.esAlarmaSonidoActivada();

            chkAlarmaSonido.Checked = alarmaSonidoActivada;

            bool alarmaProximidadActivada = configuracion.esAlarmaProximidadActivada();

            chkAlarmaProximidad.Checked = alarmaProximidadActivada;

        }

        protected void chkAlarmaTemperatura_CheckedChanged(object sender, EventArgs e)
        {            
            txtTemperaturaMaxima.Enabled = alarmaTemperaturaActivada;
            txtTemperaturaMinima.Enabled = alarmaTemperaturaActivada;
            btnSetearTemperaturas.Enabled = alarmaTemperaturaActivada;

            if(alarmaTemperaturaActivada)
            {
                ConectorMQTT.Instancia().publicar(TOPIC_ACTIVAR_TEMPERATURA, "1");
            }
            else
            {
                ConectorMQTT.Instancia().publicar(TOPIC_ACTIVAR_TEMPERATURA, "0");
                txtTemperaturaMinima.Text = "";
                txtTemperaturaMaxima.Text = "";
            }

            obtenerConfiguracionESP32(true);
        }

        protected void chkAlarmaSonido_CheckedChanged(object sender, EventArgs e)
        {
            if(alarmaSonidoActivada)
            {
                ConectorMQTT.Instancia().publicar(TOPIC_ACTIVAR_SONIDO, "1");
            }
            else
            {
                ConectorMQTT.Instancia().publicar(TOPIC_ACTIVAR_SONIDO, "0");
            }

            obtenerConfiguracionESP32(true);
        }

        protected void chkAlarmaProximidad_CheckedChanged(object sender, EventArgs e)
        {
            if(alarmaProximidadActivada)
            {
                ConectorMQTT.Instancia().publicar(TOPIC_ACTIVAR_PROXIMIDAD, "1");
            }
            else
            {
                ConectorMQTT.Instancia().publicar(TOPIC_ACTIVAR_PROXIMIDAD, "0");
            }

            obtenerConfiguracionESP32(true);
        }

        protected void btnSetearTemperaturas_Click(object sender, EventArgs e)
        {
            if(!string.IsNullOrEmpty(temperaturaMinima) && !string.IsNullOrEmpty(temperaturaMaxima))
            {
                ConectorMQTT.Instancia().publicar(TOPIC_TEMPERATURA_MINIMA, temperaturaMinima);
                ConectorMQTT.Instancia().publicar(TOPIC_TEMPERATURA_MAXIMA, temperaturaMaxima);

                obtenerConfiguracionESP32(true);
            }
            else
            {
                ScriptManager.RegisterStartupScript(this, this.GetType(), "alertCamposVacios", "alert('Debe ingresar la temperatura a asignar');", true);
            }
        }

        public void btnActualizar_Click(object sender, EventArgs e)
        {
            obtenerConfiguracionESP32(true);
        }

    }
}