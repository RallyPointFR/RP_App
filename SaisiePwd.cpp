#include "SaisiePwd.h"
#include "ui_SaisiePwd.h"

SaisiePwd::SaisiePwd(bool incorrect, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaisiePwd)
{
    ui->setupUi(this);

    //Si incorrect est à false, on cache le texte rouge, sinon on l'affiche
    if(!incorrect)
        ui->label->hide();
}

SaisiePwd::~SaisiePwd()
{
    delete ui;
}

QString SaisiePwd::getPwd()
{
    return ui->lineEdit->text();
}
