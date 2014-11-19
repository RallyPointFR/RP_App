#include "mail.h"
#include "ui_mail.h"

mail::mail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mail)
{
    ui->setupUi(this);
}

mail::~mail()
{
    delete ui;
}

QString mail::getMail()
{
    return QString(ui->nom->text() + "@" + ui->domaine->text());
}

void mail::clear()
{
    ui->nom->clear();
    ui->domaine->clear();
}
