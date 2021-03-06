USE [MqttDB]
GO
/****** Object:  Table [dbo].[Mensajes]    Script Date: 20/2/2021 20:20:01 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Mensajes](
	[id_mensaje] [int] IDENTITY(1,1) NOT NULL,
	[mensaje] [varchar](500) NULL,
	[topic] [varchar](100) NULL,
	[qosLevel] [int] NULL,
	[fecha] [datetime] NULL,
	[cliente] [varchar](100) NULL,
 CONSTRAINT [PK_Mensajes] PRIMARY KEY CLUSTERED 
(
	[id_mensaje] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  StoredProcedure [dbo].[Mensajes_getByTopic]    Script Date: 20/2/2021 20:20:01 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		Brizuela Marcelo
-- Create date: 15/02/2021
-- =============================================
CREATE PROCEDURE [dbo].[Mensajes_getByTopic]
	@topic			varchar(100),
	@fecha_desde	datetime,
	@fecha_hasta	datetime
AS
BEGIN
	SELECT *
	FROM Mensajes
	WHERE topic = @topic
	and fecha between @fecha_desde and @fecha_hasta
	order by id_mensaje desc
END
GO
/****** Object:  StoredProcedure [dbo].[Mensajes_getByTopicAfterMoment]    Script Date: 20/2/2021 20:20:01 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		Brizuela Marcelo
-- Create date: 17/02/2021
-- =============================================
CREATE PROCEDURE [dbo].[Mensajes_getByTopicAfterMoment]
	@moment		datetime,
	@topic		varchar(100)

AS
BEGIN
	SELECT TOP 1 * FROM Mensajes
	WHERE topic = @topic and fecha > @moment
	order by fecha
END
GO
/****** Object:  StoredProcedure [dbo].[MensajesInsert]    Script Date: 20/2/2021 20:20:01 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		Brizuela Marcelo
-- Create date: 15/02/2021
-- =============================================
CREATE PROCEDURE [dbo].[MensajesInsert]
	@topic		varchar(100),
	@mensaje	varchar(500),
	@fecha		datetime
AS
BEGIN
	INSERT INTO Mensajes
		(topic, mensaje, fecha)
	VALUES
		(@topic, @mensaje, @fecha)
END
GO
