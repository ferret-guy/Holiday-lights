#Region ;**** Directives created by AutoIt3Wrapper_GUI ****
#AutoIt3Wrapper_Icon=..\..\..\..\..\Downloads\Fatcow-Farm-Fresh-Port.ico
#AutoIt3Wrapper_Outfile=SerialLEDs.exe
#AutoIt3Wrapper_Res_Comment=Test version, further control to be added. Icon via IconArchive.com
#AutoIt3Wrapper_Res_Description=Serial control of Mars Rising LEDs
#AutoIt3Wrapper_Res_Fileversion=0.1.0.0
#AutoIt3Wrapper_Res_Language=1033
#EndRegion ;**** Directives created by AutoIt3Wrapper_GUI ****
#include <CommMG.au3>
#include <GUIConstantsEx.au3>
#include <WindowsConstants.au3>
#include <MsgBoxConstants.au3>
#include <ColorConstants.au3>

Local $err
Local $isOnline = False
Local $sepRunning = False
Local $colCon[11]
Local $colVal[] = [$GUI_CHECKED,$GUI_CHECKED,$GUI_CHECKED,$GUI_CHECKED,$GUI_CHECKED,$GUI_CHECKED,$GUI_CHECKED,$GUI_CHECKED,$GUI_CHECKED,$GUI_CHECKED,$GUI_CHECKED]

GUICreate("LED Control Utility",575,320)
GUICtrlCreateGroup("Fill Strip",10,10,140,110)
$redc = GUICtrlCreateInput("0",80,30,50,20)
GUICtrlCreateUpdown($redc)
GUICtrlSetLimit($redc,255,0)
GUICtrlCreateLabel("Red:",20,32)
$grnc = GUICtrlCreateInput("0",80,60,50,20)
GUICtrlCreateUpdown($grnc)
GUICtrlSetLimit($grnc,255,0)
GUICtrlCreateLabel("Green:",20,62)
$bluc = GUICtrlCreateInput("0",80,90,50,20)
GUICtrlCreateUpdown($bluc)
GUICtrlSetLimit($bluc,255,0)
GUICtrlCreateLabel("Blue:",20,92)
GUICtrlCreateGroup("Per Pixel",10,130,140,125)
$predc = GUICtrlCreateInput("0",60,150,50,20)
GUICtrlCreateUpdown($predc)
GUICtrlSetLimit($predc,255,0)
GUICtrlCreateLabel("Red:",20,152)
$pgrnc = GUICtrlCreateInput("0",60,175,50,20)
GUICtrlCreateUpdown($pgrnc)
GUICtrlSetLimit($pgrnc,255,0)
GUICtrlCreateLabel("Green:",20,177)
$pbluc = GUICtrlCreateInput("0",60,200,50,20)
GUICtrlCreateUpdown($pbluc)
GUICtrlSetLimit($pbluc,255,0)
GUICtrlCreateLabel("Blue:",20,202)
$pixelc = GUICtrlCreateInput("0",60,225,50,20)
GUICtrlCreateUpdown($pixelc)
GUICtrlSetLimit($pixelc,255,0)
GUICtrlCreateLabel("Pixel:",20,227)
$setPix = GUICtrlCreateButton("S"&@CRLF&"E"&@CRLF&"T",115,150,30,45)
$clrPix = GUICtrlCreateButton("C"&@CRLF&"L"&@CRLF&"R",115,200,30,45)
GUICtrlCreateGroup("Mode",160,10,120,110)
$radioModeAll = GUICtrlCreateRadio("Fill Strip",170,30)
$radioModeSep = GUICtrlCreateRadio("Color Separation",170,50)
$radioModePix = GUICtrlCreateRadio("Per Pixel",170,70)
$clrStrip = GUICtrlCreateButton("Clear Strip",170,95,100,20)
GUICtrlSetState($radioModeSep,$GUI_CHECKED)
GUICtrlCreateGroup("Color Separation",290,10,140,210)
;TIME CONTROL REPURPOSED TO DELAY
$timec = GUICtrlCreateInput("100",360,60,50,20)
GUICtrlCreateLabel("Delay (ms):",300,62)
GUICtrlCreateUpdown($timec)
GUICtrlSetLimit($timec,65535,0)
$modec = GUICtrlCreateInput("1",360,30,50,20)
GUICtrlCreateLabel("Mode:",300,32)
GUICtrlCreateUpdown($modec)
GUICtrlSetLimit($modec,3,0)
$runSep = GUICtrlCreateButton("Run",300,90,120,20)
$endSep = GUICtrlCreateButton("Stop",300,120,120,20)
$colCon[0] = GUICtrlCreateCheckbox("RED",295,144,40,20)
$colCon[1] = GUICtrlCreateCheckbox("GRN",340,144,40,20)
$colCon[2] = GUICtrlCreateCheckbox("BLU",385,144,40,20)
$colCon[3] = GUICtrlCreateCheckbox("WHT",295,162,40,20)
$colCon[4] = GUICtrlCreateCheckbox("ORN",340,162,40,20)
$colCon[5] = GUICtrlCreateCheckbox("YLO",385,162,40,20)
$colCon[6] = GUICtrlCreateCheckbox("PUR",295,180,40,20)
$colCon[7] = GUICtrlCreateCheckbox("WRW",340,180,40,20)
$colCon[8] = GUICtrlCreateCheckbox("TRQ",385,180,40,20)
$colCon[9] = GUICtrlCreateCheckbox("MGN",315,198,40,20)
$colCon[10] = GUICtrlCreateCheckbox("AMB",365,198,40,20)
GUICtrlSetState($colCon[0],$GUI_CHECKED)
GUICtrlSetState($colCon[1],$GUI_CHECKED)
GUICtrlSetState($colCon[2],$GUI_CHECKED)
GUICtrlSetState($colCon[3],$GUI_CHECKED)
GUICtrlSetState($colCon[4],$GUI_CHECKED)
GUICtrlSetState($colCon[5],$GUI_CHECKED)
GUICtrlSetState($colCon[6],$GUI_CHECKED)
GUICtrlSetState($colCon[7],$GUI_CHECKED)
GUICtrlSetState($colCon[8],$GUI_CHECKED)
GUICtrlSetState($colCon[9],$GUI_CHECKED)
GUICtrlSetState($colCon[10],$GUI_CHECKED)
GUICtrlCreateGroup("Serial Control",160,130,120,125)
$conInd = GUICtrlCreateGraphic(240,150,25,95)
GUICtrlSetBkColor($conInd,$COLOR_RED)
GUICtrlSetGraphic($conInd,$GUI_GR_RECT,0,0,25,95)
$comc = GUICtrlCreateInput("5",200,150,30,20)
GUICtrlCreateLabel("COM",170,152,25,18)
GUICtrlCreateUpdown($comc)
GUICtrlSetLimit($comc,3,0)
$goOnline = GUICtrlCreateButton("Online",170,175,60,20)
$goReset = GUICtrlCreateButton("Reset",170,200,60,20)
$goOffline = GUICtrlCreateButton("Offline",170,225,60,20)
GUICtrlCreateGroup("Info",290,220,140,35)
GUICtrlCreateLabel("Mars Rising LED Utility v1.3",295,235,134,15)
GUICtrlCreateGroup("Color Separation - Update Color",10,265,420,45)
$credc = GUICtrlCreateInput("0",55,283,50,20)
GUICtrlCreateUpdown($credc)
GUICtrlSetLimit($credc,255,0)
GUICtrlCreateLabel("Red:",20,285)
$cgrnc = GUICtrlCreateInput("0",150,283,50,20)
GUICtrlCreateUpdown($cgrnc)
GUICtrlSetLimit($cgrnc,255,0)
GUICtrlCreateLabel("Green:",110,285)
$cbluc = GUICtrlCreateInput("0",240,283,50,20)
GUICtrlCreateUpdown($cbluc)
GUICtrlSetLimit($cbluc,255,0)
GUICtrlCreateLabel("Blue:",205,285)
$colorc = GUICtrlCreateInput("0",330,283,50,20)
GUICtrlCreateUpdown($colorc)
GUICtrlSetLimit($colorc,255,0)
GUICtrlCreateLabel("Color:",295,285)
$setCol = GUICtrlCreateButton("S"&@CRLF&"E"&@CRLF&"T",385,280,40,26)
GUICtrlCreateGroup("Remote",440,10,125,210)
$remOn = GUICtrlCreateButton("Powr",450,25,35,30)
GUICtrlSetBkColor ( $remOn, 0x00FF00)
$remSpeUp = GUICtrlCreateButton("Spe+",487,25,35,30)
GUICtrlSetBkColor ( $remSpeUp, 0xEEEEEE)
$remOff = GUICtrlCreateButton("Off",524,25,35,30)
GUICtrlSetBkColor ( $remOff, 0xFF0000)
$remBriUp = GUICtrlCreateButton("Bri+",450,57,35,30)
GUICtrlSetBkColor ( $remBriUp, 0xEEEEEE)
$remSpeDown = GUICtrlCreateButton("Spe-",487,57,35,30)
GUICtrlSetBkColor ( $remSpeDown, 0xEEEEEE)
$remBriDown = GUICtrlCreateButton("Bri-",524,57,35,30)
GUICtrlSetBkColor ( $remBriDown, 0xEEEEEE)
$remColMGM = GUICtrlCreateButton("Mage",450,89,35,30)
GUICtrlSetBkColor ( $remColMGM, 0xFF00FF)
$remMode = GUICtrlCreateButton("Mode",487,89,35,30)
GUICtrlSetBkColor ( $remMode, 0xEEEEEE)
$remColWHT = GUICtrlCreateButton("Whit",524,89,35,30)
GUICtrlSetBkColor ( $remColWHT, 0xFFFFFF)
$remColWRW = GUICtrlCreateButton("WmWh",450,121,35,30)
GUICtrlSetBkColor ( $remColWRW, 0xFFEAC4)
$remColAMB = GUICtrlCreateButton("Ambr",487,121,35,30)
GUICtrlSetBkColor ( $remColAMB, 0xFFC819)
$remColTRQ = GUICtrlCreateButton("Turq",524,121,35,30)
GUICtrlSetBkColor ( $remColTRQ, 0x40E0D0)
$remColORM = GUICtrlCreateButton("Oran",450,151,35,30)
GUICtrlSetBkColor ( $remColORM, 0xFFA500)
$remColYLO = GUICtrlCreateButton("Yell",487,151,35,30)
GUICtrlSetBkColor ( $remColYLO, 0xFFFF00)
$remColPRP = GUICtrlCreateButton("Purp",524,151,35,30)
GUICtrlSetBkColor ( $remColPRP, 0x800080)
$remColRED = GUICtrlCreateButton("Red",450,183,35,30)
GUICtrlSetBkColor ( $remColRED, 0xFF0000)
$remColBLU = GUICtrlCreateButton("Blue",487,183,35,30)
GUICtrlSetBkColor ( $remColBLU, 0x0000FF)
$remColGRN = GUICtrlCreateButton("Green",524,183,35,30)
GUICtrlSetBkColor ( $remColGRN, 0x00FF00)
GUICtrlCreateGroup("Brightness",440,265,125,45)
$bric = GUICtrlCreateInput("255",450,283,60,20)
GUICtrlCreateUpdown($bric)
$doBri = GUICtrlCreateButton("SET",515,282,45,22)

GUISetState(@SW_SHOW)

Func serialOn()
	$comn=GUICtrlRead($comc)
	If $comn>255 Then
		GUICtrlSetData($comc,255)
		$comn=GUICtrlRead($comc)
	EndIf
	If $comn<0 Then
		GUICtrlSetData($comc,0)
		$comn=GUICtrlRead($comc)
	EndIf
	_CommSetPort($comn,$err)
	Sleep(1250)
	$isOnline=True
	GUICtrlSetBkColor($conInd,$COLOR_GREEN)
	GUICtrlSetGraphic($conInd,$GUI_GR_RECT,0,0,25,95)
EndFunc

Func serialReset()
	$comn=GUICtrlRead($comc)
	If $comn>255 Then
		GUICtrlSetData($comc,255)
		$comn=GUICtrlRead($comc)
	EndIf
	If $comn<0 Then
		GUICtrlSetData($comc,0)
		$comn=GUICtrlRead($comc)
	EndIf
	GUICtrlSetBkColor($conInd,$COLOR_YELLOW)
	GUICtrlSetGraphic($conInd,$GUI_GR_RECT,0,0,25,95)
	_CommClosePort()
	Sleep(1250)
	_CommSetPort($comn,$err)
	Sleep(1250)
	$sepRunning=False
	GUICtrlSetBkColor($conInd,$COLOR_GREEN)
	GUICtrlSetGraphic($conInd,$GUI_GR_RECT,0,0,25,95)
EndFunc

Func serialOff()
	_CommClosePort()
	Sleep(1250)
	$isOnline=False
	GUICtrlSetBkColor($conInd,$COLOR_RED)
	GUICtrlSetGraphic($conInd,$GUI_GR_RECT,0,0,25,95)
EndFunc

Func DoSetBrightness()
    $brin=GUICtrlRead($bric)
	If $brin>255 Then
		GUICtrlSetData($bric,255)
		$brin=GUICtrlRead($bric)
	EndIf
	If $brin<0 Then
		GUICtrlSetData($bric,0)
		$brin=GUICtrlRead($bric)
	EndIf
	_CommSendByte(253)
	_CommSendByte(2)
	_CommSendByte(21)
	_CommSendByte($brin)
EndFunc

Func DoModeAll()
	;While 1
		;Switch GUIGetMsg()
		;	Case $GUI_EVENT_CLOSE
		;		ExitLoop
		;	Case $goOffline
		;		serialOff()
		;		ExitLoop
		;EndSwitch
		;If GUICtrlRead($radioModeAll)==$GUI_UNCHECKED Then
		;	ExitLoop
		;EndIf
		;If $isOnline==False Then
		;	ExitLoop
		;EndIf
		$redn=GUICtrlRead($redc)
		If $redn>255 Then
			GUICtrlSetData($redc,255)
			$redn=GUICtrlRead($redc)
		EndIf
		If $redn<0 Then
			GUICtrlSetData($redc,0)
			$redn=GUICtrlRead($redc)
		EndIf
		$grnn=GUICtrlRead($grnc)
		If $grnn>255 Then
			GUICtrlSetData($grnc,255)
			$grnn=GUICtrlRead($grnc)
		EndIf
		If $grnn<0 Then
			GUICtrlSetData($grnc,0)
			$grnn=GUICtrlRead($grnc)
		EndIf
		$blun=GUICtrlRead($bluc)
		If $blun>255 Then
			GUICtrlSetData($bluc,255)
			$blun=GUICtrlRead($bluc)
		EndIf
		If $blun<0 Then
			GUICtrlSetData($bluc,0)
			$blun=GUICtrlRead($bluc)
		EndIf
		_CommSendByte(253)
		_CommSendByte(4)
		_CommSendByte(44)
		_CommSendByte(GUICtrlRead($redc))
		_CommSendByte(GUICtrlRead($grnc))
		_CommSendByte(GUICtrlRead($bluc))
		;_CommSendByte(0)
		;_CommSendByte(0)
		;_CommSendByte(254)
		Sleep(25)
	;Wend
EndFunc

Func DoColorUpdate()
	$credn=GUICtrlRead($credc)
	If $credn>255 Then
		GUICtrlSetData($credc,255)
		$credn=GUICtrlRead($credc)
	EndIf
	If $credn<0 Then
		GUICtrlSetData($credc,0)
		$credn=GUICtrlRead($credc)
	EndIf
	$cgrnn=GUICtrlRead($cgrnc)
	If $cgrnn>255 Then
		GUICtrlSetData($cgrnc,255)
		$cgrnn=GUICtrlRead($cgrnc)
	EndIf
	If $cgrnn<0 Then
		GUICtrlSetData($cgrnc,0)
		$gcrnn=GUICtrlRead($cgrnc)
	EndIf
	$cblun=GUICtrlRead($cbluc)
	If $cblun>255 Then
		GUICtrlSetData($cbluc,255)
		$cblun=GUICtrlRead($cbluc)
	EndIf
	If $cblun<0 Then
		GUICtrlSetData($cbluc,0)
		$cblun=GUICtrlRead($cbluc)
	EndIf
	$colorn=GUICtrlRead($colorc)
	If $colorn>255 Then
		GUICtrlSetData($colorc,255)
		$colorn=GUICtrlRead($colorc)
	EndIf
	If $colorn<0 Then
		GUICtrlSetData($colorc,0)
		$colorn=GUICtrlRead($colorc)
	EndIf
	_CommSendByte(253)
	_CommSendByte(6)
	_CommSendByte(50)
	_CommSendByte(GUICtrlRead($credc))
	_CommSendByte(GUICtrlRead($cgrnc))
	_CommSendByte(GUICtrlRead($cbluc))
	_CommSendByte(GUICtrlRead($colorc))
	_CommSendByte(0)
	;_CommSendByte(254)
	Sleep(25)
EndFunc

Func DoClearStrip()
	_CommSendByte(253)
	_CommSendByte(4)
	_CommSendByte(44)
	_CommSendByte(0)
	_CommSendByte(0)
	_CommSendByte(0)
	;_CommSendByte(0)
	;_CommSendByte(0)
	;_CommSendByte(254)
EndFunc

Func DoModePix($submode)
	$predn=GUICtrlRead($predc)
	If $predn>255 Then
		GUICtrlSetData($predc,255)
		$predn=GUICtrlRead($predc)
	EndIf
	If $predn<0 Then
		GUICtrlSetData($predc,0)
		$predn=GUICtrlRead($predc)
	EndIf
	$pgrnn=GUICtrlRead($pgrnc)
	If $pgrnn>255 Then
		GUICtrlSetData($pgrnc,255)
		$pgrnn=GUICtrlRead($pgrnc)
	EndIf
	If $pgrnn<0 Then
		GUICtrlSetData($pgrnc,0)
		$pgrnn=GUICtrlRead($pgrnc)
	EndIf
	$pblun=GUICtrlRead($pbluc)
	If $pblun>255 Then
		GUICtrlSetData($pbluc,255)
		$pblun=GUICtrlRead($pbluc)
	EndIf
	If $pblun<0 Then
		GUICtrlSetData($pbluc,0)
		$pblun=GUICtrlRead($pbluc)
	EndIf
	;If Not (60<=$submode<=62) Then
	;	$submode=60
	;EndIf
	$pixeln=GUICtrlRead($pixelc)
	If $pixeln>65535 Then
		GUICtrlSetData($pixelc,65535)
		$pixeln=GUICtrlRead($pixelc)
	EndIf
	If $pixeln<0 Then
		GUICtrlSetData($pixelc,0)
		$pixeln=GUICtrlRead($pixelc)
	EndIf
	$pixelh=BitAND(BitShift($pixeln,8),255)
	$pixell=BitAND($pixeln,255)
	_CommSendByte(253)
	_CommSendByte(6)
	_CommSendByte($submode)
	_CommSendByte(GUICtrlRead($predc))
	_CommSendByte(GUICtrlRead($pgrnc))
	_CommSendByte(GUICtrlRead($pbluc))
	_CommSendByte($pixelh)
	_CommSendByte($pixell)
	;_CommSendByte(254)
	Sleep(25)
EndFunc

Func DoSetPix()
	DoModePix(61)
EndFunc

Func DoClearPix()
	DoModePix(62)
EndFunc

Func DoPreviewPix()
	DoModePix(60)
EndFunc

Func DoChangeCol($col,$case)
	_CommSendByte(253)
	_CommSendByte(3)
	_CommSendByte(30)
	_CommSendByte($col)
	_CommSendByte($case)
	;_CommSendByte(0)
	;_CommSendByte(0)
	;_CommSendByte(0)
	;_CommSendByte(254)
EndFunc

Func DoOnCol($col)
	DoChangeCol($col,1)
EndFunc

; Start remote emulation*****************

Func Remote_On()
	_CommSendByte(253)
	_CommSendByte(1)
	_CommSendByte(100)
	;_CommSendByte(254)
EndFunc

Func Remote_SpeUp()
	_CommSendByte(253)
	_CommSendByte(1)
	_CommSendByte(101)
	;_CommSendByte(254)
EndFunc

Func Remote_Off()
	;_CommSendByte(253)
	;_CommSendByte(1)
	_CommSendByte(102)
	;_CommSendByte(254)
EndFunc

Func Remote_BriUp()
	_CommSendByte(253)
	_CommSendByte(1)
	_CommSendByte(104)
	;_CommSendByte(254)
EndFunc

Func Remote_SpeDown()
	_CommSendByte(253)
	_CommSendByte(1)
	_CommSendByte(105)
	;_CommSendByte(254)
EndFunc

Func Remote_BriDown()
	_CommSendByte(253)
	_CommSendByte(1)
	_CommSendByte(106)
	;_CommSendByte(254)
EndFunc

Func Remote_ColMGM()
	_CommSendByte(253)
	_CommSendByte(1)
	_CommSendByte(108)
	;_CommSendByte(254)
EndFunc

Func Remote_Mode()
	_CommSendByte(253)
	_CommSendByte(1)
	_CommSendByte(109)
	;_CommSendByte(254)
EndFunc

Func Remote_ColWHT()
	_CommSendByte(253)
	_CommSendByte(1)
	_CommSendByte(110)
	;_CommSendByte(254)
EndFunc

Func Remote_ColWRW()
	_CommSendByte(253)
	_CommSendByte(1)
	_CommSendByte(112)
	;_CommSendByte(254)
EndFunc

Func Remote_ColAMB()
	_CommSendByte(253)
	_CommSendByte(1)
	_CommSendByte(113)
	;_CommSendByte(254)
EndFunc

Func Remote_ColTRQ()
	_CommSendByte(253)
	_CommSendByte(1)
	_CommSendByte(114)
	;_CommSendByte(254)
EndFunc

Func Remote_ColORM()
	_CommSendByte(253)
	_CommSendByte(1)
	_CommSendByte(116)
	;_CommSendByte(254)
EndFunc

Func Remote_ColYLO()
	_CommSendByte(253)
	_CommSendByte(1)
	_CommSendByte(117)
	;_CommSendByte(254)
EndFunc

Func Remote_ColPRP()
	_CommSendByte(253)
	_CommSendByte(1)
	_CommSendByte(118)
	;_CommSendByte(254)
EndFunc

Func Remote_ColRED()
	_CommSendByte(253)
	_CommSendByte(1)
	_CommSendByte(120)
	;_CommSendByte(254)
EndFunc

Func Remote_ColBLU()
	_CommSendByte(253)
	_CommSendByte(1)
	_CommSendByte(121)
	;_CommSendByte(254)
EndFunc

Func Remote_ColGRN()
	_CommSendByte(253)
	_CommSendByte(1)
	_CommSendByte(122)
	;_CommSendByte(254)
EndFunc
; End remote emulation*******************

Func DoOffCol($col)
	DoChangeCol($col,0)
EndFunc

Func DoUpdateCol()
	;ConsoleWrite("UpdateCol Runs"&@CRLF)
	For $ccol = 0 To 10
		;ConsoleWrite($ccol)
		$ccolv = GUICtrlRead($colCon[$ccol])
		;ConsoleWrite($GUI_CHECKED)
		;ConsoleWrite($ccolv)
		;ConsoleWrite(@CRLF)
		;If Not($ccolv==$colVal[$ccol]) Then
			If $ccolv==$GUI_CHECKED Then
				DoOnCol($ccol)
			ElseIf $ccolv==$GUI_UNCHECKED Then
				DoOffCol($ccol)
			EndIf
		;EndIf
	Next
EndFunc


Func DoModeSep()
	;TIME CONTROL REPURPOSED TO DELAY
	$timen=GUICtrlRead($timec)
	If $timen>65535 Then
		GUICtrlSetData($timec,65535)
		$timen=GUICtrlRead($timec)
	EndIf
	If $timen<0 Then
		GUICtrlSetData($timec,0)
		$timen=GUICtrlRead($timec)
	EndIf
	$moden=GUICtrlRead($modec)
	If $moden>255 Then
		GUICtrlSetData($modec,255)
		$moden=GUICtrlRead($modec)
	EndIf
	If $moden<0 Then
		GUICtrlSetData($modec,0)
		$moden=GUICtrlRead($modec)
	EndIf
	$timeh=BitAND(BitShift($timen,8),255)
	$timel=BitAND($timen,255)
	;$timeh=30
	;$timel=30
	_CommSendByte(253)
	_CommSendByte(4)
	_CommSendByte(40)
	_CommSendByte($timeh)
	_CommSendByte($timel)
	_CommSendByte($moden)
	;_CommSendByte(0)
	;_CommSendByte(0)
	;_CommSendByte(254)
	$sepRunning=True
	Sleep(25)
EndFunc

Func DoKillSep()
	;_CommSendByte(253)
	;_CommSendByte(1)
	_CommSendByte(31)
	;_CommSendByte(0)
	;_CommSendByte(0)
	;_CommSendByte(0)
	;_CommSendByte(0)
	;_CommSendByte(0)
	;_CommSendByte(254)
	$sepRunning=False
	Sleep(15)
EndFunc

While 1
	;$msg = _CommGetLine(@CRLF,0,50)
	;If @error==0 and Not($msg=="" or $msg="0") Then
	;	ConsoleWrite($msg&@CRLF)
	;EndIf
	If $isOnline==True And $sepRunning==False Then
		Switch GUIGetMsg()
			;Case $radioModeAll
			;	DoModeAll()
			Case $runSep
				If GUICtrlRead($radioModeSep)==$GUI_CHECKED Then
					DoUpdateCol()
					DoModeSep()
				EndIf
			Case $endSep
				If GUICtrlRead($radioModeSep)==$GUI_CHECKED Then
					DoKillSep()
				EndIf
			Case $setPix
				DoSetPix()
			Case $clrPix
				DoClearPix()
			Case $goOffline
				serialOff()
			Case $goReset
				serialReset()
			Case $clrStrip
				DoClearStrip()
			Case $setCol
				DoColorUpdate()
			Case $remOn
				Remote_On()
			Case $remSpeUp
				Remote_SpeUp()
			Case $remOff
				Remote_Off()
			Case $remBriUp
				Remote_BriUp()
			Case $remSpeDown
				Remote_SpeDown()
			Case $remBriDown
				Remote_BriDown()
			Case $remColMGM
				Remote_ColMGM()
			Case $remColWHT
				Remote_ColWHT()
			Case $remColWRW
				Remote_ColWRW()
			Case $remColAMB
				Remote_ColAMB()
			Case $remColTRQ
				Remote_ColTRQ()
			Case $remColORM
				Remote_ColORM()
			Case $remColYLO
				Remote_ColYLO()
			Case $remColPRP
				Remote_ColPRP()
			Case $remColRED
				Remote_ColRED()
			Case $remColBLU
				Remote_ColBLU()
			Case $remColGRN
				Remote_ColGRN()
			Case $remMode
				Remote_Mode()
			Case $doBri
				DoSetBrightness()
			Case $GUI_EVENT_CLOSE
				ExitLoop
		EndSwitch
		If GUICtrlRead($radioModeAll)==$GUI_CHECKED Then
			DoModeAll()
		EndIf
		If GUICtrlRead($radioModePix)==$GUI_CHECKED Then
			DoPreviewPix()
		EndIf
		;If GUICtrlRead($radioModeSep)==$GUI_CHECKED Then
		;	DoUpdateCol()
		;EndIf
	ElseIf $isOnline==True And $sepRunning==True Then
		Switch GUIGetMsg()
			Case $endSep
				If GUICtrlRead($radioModeSep)==$GUI_CHECKED Then
					DoKillSep()
				EndIf
			Case $goReset
				serialReset()
			Case $GUI_EVENT_CLOSE
				ExitLoop
		EndSwitch
		If GUICtrlRead($radioModeSep)==$GUI_UNCHECKED Then
			GUICtrlSetState($radioModeSep,$GUI_CHECKED)
		EndIf
	Else
		Switch GUIGetMsg()
			Case $goOnline
				serialOn()
			Case $GUI_EVENT_CLOSE
				ExitLoop
		EndSwitch
	EndIf
	;Sleep(5)
WEnd
