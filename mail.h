#ifndef MAIL_H
#define MAIL_H

#include <QWidget>

namespace Ui {
class mail;
}

class mail : public QWidget
{
    Q_OBJECT

public:
    explicit mail(QWidget *parent = 0);
    ~mail();

    QString getMail();
    void clear();

private:
    Ui::mail *ui;
};

#endif // MAIL_H
