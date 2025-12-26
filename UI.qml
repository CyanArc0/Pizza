import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
//import QtQuick.Controls.Material

ApplicationWindow {
    //Material.theme: Material.Light
    //Material.accent: Material.Blue
    id: mainWindow
    width: 360
    height: 640
    visible: true
    title: "Pizza"
    ///*
    Rectangle
    {
        width: parent.width
        height: parent.height
        gradient: Gradient
        {
            orientation: Gradient.Vertical
            GradientStop
            {
                position: 0.0
                color:"#F4A73A"//融化芝士黄
            }
            GradientStop
            {
                position: 1.0
                color: "#C5281C"//浓郁番茄红
            }
        }
    }
    //*/
    Connections
    {
        target: bridge
        function onShowMessage(title,text)
        {
            messageDialog.title = title
            messageDialog.text = text
            messageDialog.open()
        }
        function onTaskFinished(sign)
        {
            if(sign)
            {
                progressBar.value = 1.0
            }
            else
            {
                progressBar.value = 0.0
            }
            progressBar.indeterminate = false
        }
    }
    MessageDialog
    {
        id: messageDialog
    }
    Button
    {
        text: '?'
        width: height
        onClicked:
        {
            myDialog.open()
        }
    }
    Dialog {
        id: myDialog
        modal: true
        contentItem:Text {
            text: "<h3>Pizza 1.1</h3><p>Pre: .png .jpg ...<br>Suf: .zip</p><a href='https://github.com/CyanArc0/Pizza'>github.com/CyanArc0/Pizza</a>"
            // 告诉Text组件它包含HTML和链接
            textFormat: Text.RichText
            // 默认链接颜色和风格
            // 处理链接点击事件
            onLinkActivated:(url) => {
                // 可以在这里调用C++函数或打开网页
                Qt.openUrlExternally(url); // QML内建函数打开URL
            }
        }
    }
    Column
    {
        spacing: 10
        anchors.centerIn: parent
        Row
        {
            anchors.horizontalCenter: parent.horizontalCenter
            Image
            {
                id: pImage
                width: 200
                height: 200
                source: pfilePathField.text
                fillMode: Image.PreserveAspectFit
            }
        }
        Row
        {
            anchors.horizontalCenter: parent.horizontalCenter
            TextField
            {
                id: pfilePathField
                width: 300
                verticalAlignment: Text.AlignVCenter
                placeholderText: "Pre-file-URI"
            }
            Button
            {
                text: "..."
                height: pfilePathField.height
                onClicked:
                {
                    pfileDialog.open()
                }
            }
            FileDialog
            {
                id: pfileDialog
                //folder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]
                nameFilters: [
                    "图片文件 (*.png *.jpg)",
                    "所有文件 (*)"
                ]
                onAccepted:
                {
                    pfilePathField.text = pfileDialog.selectedFile
                    //console.log("选择的文件:", pfileDialog.selectedFile)
                }
            }
        }
        Row
        {
            anchors.horizontalCenter: parent.horizontalCenter
            Label
            {
                text: '+'
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
        Row
        {
            anchors.horizontalCenter: parent.horizontalCenter
            TextField
            {
                id: zfilePathField
                width: 300
                verticalAlignment: Text.AlignVCenter
                placeholderText: "Suf-file-URI"
            }
            Button
            {
                text: "..."
                height: zfilePathField.height
                onClicked:
                {
                    zfileDialog.open()
                }
            }
            FileDialog
            {
                id: zfileDialog
                //folder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]
                nameFilters: [
                    "压缩文件 (*.zip)",
                    "所有文件 (*)"
                ]
                onAccepted:
                {
                    zfilePathField.text = zfileDialog.selectedFile
                    //console.log("选择的文件:", zfileDialog.selectedFile)
                }
            }
        }
        Row
        {
            anchors.horizontalCenter: parent.horizontalCenter
            Button
            {
                text: '='
                width: height
                //font.pointSize: 12
                onClicked:
                {
                    if(pfilePathField.text==="")
                    {
                        bridge.showMessage("Error","Pre-file-URI is empty.")
                    }
                    else if(zfilePathField.text==="")
                    {
                        bridge.showMessage("Error","Suf-file-URI is empty.")
                    }
                    else if(progressBar.indeterminate)
                    {
                        bridge.showMessage("Error","The core is working...")
                    }
                    else ofileDialog.open()
                }
            }
            FileDialog {
                id: ofileDialog
                fileMode: FileDialog.SaveFile
                //folder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]
                currentFile: zfilePathField.text.slice(zfilePathField.text.lastIndexOf('/')+1,zfilePathField.text.lastIndexOf('.')) + pfilePathField.text.slice(pfilePathField.text.lastIndexOf('.'))
                nameFilters: [
                    "图片文件 (*.png *.jpg)",
                    "压缩文件 (*.zip)",
                    "所有文件 (*)"
                ]
                onAccepted:
                {
                    ofilePathField.text = ofileDialog.selectedFile
                    //console.log("选择的文件:", ofileDialog.selectedFile)
                    bridge.invokeMerge(pfilePathField.text,zfilePathField.text,ofilePathField.text)
                    progressBar.value = 0.0
                    progressBar.indeterminate = true

                }
            }
        }
        Row
        {
            anchors.horizontalCenter: parent.horizontalCenter
            /*
            Button
            {
                text: "->"
                height: ofilePathField.height
                onClicked:
                {
                    Qt.openUrlExternally(ofilePathField.text)
                }
            }
            */
            TextField
            {
                id: ofilePathField
                width: 360
                verticalAlignment: Text.AlignVCenter
                readOnly: true
            }
        }
        Row
        {
            anchors.horizontalCenter: parent.horizontalCenter
            ProgressBar {
                id: progressBar
                width: 360
                value: 0.0
                indeterminate: false
            }
        }
    }
}
