<%@ Page Title="Configuración" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="Configuracion.aspx.cs" Inherits="Estadisticas.Configuracion" %>
<asp:Content ID="Content1" ContentPlaceHolderID="MainContent" runat="server">

    <fieldset>
        <legend>Temperatura</legend>
        <div class="row">
            <div class="col-3 mt-4">
                <asp:CheckBox ID="chkAlarmaTemperatura" runat="server" AutoPostBack="true" OnCheckedChanged="chkAlarmaTemperatura_CheckedChanged" />
                <asp:Label ID="lblChkAlarmaTemperatura" runat="server" AssociatedControlID="chkAlarmaTemperatura">Activar alarma temperatura</asp:Label>                
            </div>
            <div class="col-3">
                <asp:Label ID="lblTemperaturaMinima" runat="server" AssociatedControlID="txtTemperaturaMinima">Temperatura Mínima</asp:Label>
                <asp:TextBox ID="txtTemperaturaMinima" runat="server" CssClass="form-control" Enabled="false" />
                
            </div>
            <div class="col-3">
                <asp:Label ID="lblTemperaturaMaxima" runat="server" AssociatedControlID="txtTemperaturaMaxima">Temperatura Máxima</asp:Label>
                <asp:TextBox ID="txtTemperaturaMaxima" runat="server" CssClass="form-control" Enabled="false" />
            </div>
            <div class="col-3 mt-4">
                <asp:Button ID="btnSetearTemperaturas" runat="server" Text="Guardar" OnClick="btnSetearTemperaturas_Click" Enabled="false" />
            </div>
        </div>
    </fieldset>
    

    <fieldset>
        <legend>Sonido</legend>
        <div class="row">
            <div class="col-3">
                <asp:CheckBox ID="chkAlarmaSonido" runat="server" AutoPostBack="true" OnCheckedChanged="chkAlarmaSonido_CheckedChanged"/>
                <asp:Label ID="lblChkAlarmaSonido" runat="server" AssociatedControlID="chkAlarmaSonido">Activar alarma sonido</asp:Label>
                
            </div>
        </div>
    </fieldset>

    <fieldset>
        <legend>Proximidad</legend>
        <div class="row">
            <div class="col-3">
                <asp:CheckBox ID="chkAlarmaProximidad" runat="server" AutoPostBack="true" OnCheckedChanged="chkAlarmaProximidad_CheckedChanged" />
                <asp:Label ID="lblChkAlarmaProximidad" runat="server" AssociatedControlID="chkAlarmaProximidad">Activar alarma proximidad</asp:Label>
                
            </div>
        </div>
    </fieldset>

    <asp:Button ID="btnActualizar" runat="server" Text="Refrescar" CssClass="btn btn-secondary col-1 mt-2" OnClick="btnActualizar_Click" />

    <script>
     $(document).ready(function () {
            $('#hrefConfiguracion').addClass('active');
     });
    </script>

</asp:Content>
