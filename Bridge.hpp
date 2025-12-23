#ifndef BRIDGE_H
#define BRIDGE_H

#include <QQuickItem>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <cstdio>
#include <unistd.h>  // 提供 dup() 函数
#include <expected>
#include <string>
#include "string_traits/struct_string.hpp"
#include "bytes/file.hpp"

extern std::expected<void,std::string> merge(file_view,file_view,file_view);

class Bridge : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
public:
    Bridge(){}
    static QString filePath(QUrl uri)
    {
        if(uri.isLocalFile())
            return uri.toLocalFile();
        else return uri.toString();
    }
    template<file::open_mode myom,QIODevice::OpenModeFlag qomf,struct_string cmss>
    static std::expected<file,std::string> openFile(QUrl uri)
    {
        if(uri.isLocalFile())
        {
            file f(uri.toLocalFile().toStdString(),myom);
            if(!f)[[unlikely]] return std::unexpected("@openFile:+1");
            return std::move(f);
        }
        else//Android
        {
            QFile file(uri.toString());

            // 1. 必须先用 QFile 打开，Android 才会授予底层 FD 的访问权
            // 这里的模式应与 mode 参数对应
            if (!file.open(qomf))[[unlikely]] return std::unexpected("@openFile:-1");

            // 2. 获取原始文件描述符
            int originalFd = file.handle();
            if (originalFd == -1)[[unlikely]] return std::unexpected("@openFile:-2");

            // 3. 使用 dup() 复制描述符
            // 这样会产生一个新的 FD 指向同一个文件位置，但生命周期与 QFile 脱离
            int newFd = dup(originalFd);
            if (newFd == -1)[[unlikely]] return std::unexpected("@openFile:-3");

            // 4. 将新 FD 转换为 FILE*
            FILE* fp = fdopen(newFd, cmss.c_str());
            if (!fp)[[unlikely]]
            {
                ::close(newFd); // 转换失败需手动关闭，防止 FD 泄漏
                return std::unexpected("@openFile:-4");
            }

            // 5. 此时 file 对象可以超出作用域被析构
            // file.close() 只会关闭 originalFd，不影响我们的 fp (newFd)
            return file::make(std::move(fp));
        }
    }
    static std::expected<void,std::string> mergePZ(QUrl p,QUrl z,QUrl o)
    {
        QFileInfo pi(filePath(p));
        QFileInfo zi(filePath(z));
        if(!pi.exists())[[unlikely]]return std::unexpected("@mergePZ:p");
        if(!zi.exists())[[unlikely]]return std::unexpected("@mergePZ:z");
        if(zi.suffix()!="zip")[[unlikely]]return std::unexpected("Suf-file is not .zip");

        auto _rp = openFile<file::readonly,QIODevice::ReadOnly,"rb">(p);
        if(!_rp)[[unlikely]]return std::unexpected("@mergePZ:p"+std::move(_rp).error());
        file fp = *std::move(_rp);

        auto _rz = openFile<file::readonly,QIODevice::ReadOnly,"rb">(z);
        if(!_rz)[[unlikely]]return std::unexpected("@mergePZ:z"+std::move(_rz).error());
        file fz = *std::move(_rz);

        auto _ro = openFile<file::overwrite,QIODevice::WriteOnly,"wb">(o);
        if(!_ro)[[unlikely]]return std::unexpected("@mergePZ:o"+std::move(_ro).error());
        file fo = *std::move(_ro);

        return merge(fp,fz,fo);
    }
public slots:
    Q_INVOKABLE void invokeMerge(QUrl p,QUrl z,QUrl o)
    {
        auto result = mergePZ(p,z,o);
        if(!result)[[unlikely]]
        {
            emit showMessage("Error",QString::fromStdString(result.error()));
        }
    }
signals:
    void showMessage(QString title,QString text);
};

#endif // BRIDGE_H
