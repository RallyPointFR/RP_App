#ifndef SAISIEPWD_H
#define SAISIEPWD_H

#include <QDialog>

namespace Ui {
class SaisiePwd;
}

class SaisiePwd : public QDialog
{
    Q_OBJECT

public:
    explicit SaisiePwd(bool incorrect = false, QWidget *parent = 0);
    ~SaisiePwd();

    QString getPwd();

private:
    Ui::SaisiePwd *ui;
};

#endif // SAISIEPWD_H
