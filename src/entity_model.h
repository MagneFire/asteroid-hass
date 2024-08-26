#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H

#include "entity.h"
#include <QAbstractListModel>

class EntityModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum FileRoles {
    DomainRole = Qt::UserRole + 1,
    NameRole,
    IdRole,
    StateRole
    };

    EntityModel(QObject *parent = 0);
    virtual ~EntityModel() {};

    void addFile(const Entity &file);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
signals:
    void EntityModelChanged();

protected:
    QHash<int, QByteArray> roleNames() const;
    private:
    QList<Entity> m_files;
};
#endif