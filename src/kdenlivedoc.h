/***************************************************************************
 *   Copyright (C) 2007 by Jean-Baptiste Mardelle (jb@kdenlive.org)        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/


#ifndef KDENLIVEDOC_H
#define KDENLIVEDOC_H

#include <qdom.h>
#include <QString>
#include <QMap>
#include <QList>
#include <QObject>
#include <QUndoGroup>
#include <QUndoStack>
#include <QTimer>

#include <KUrl>
#include <kautosavefile.h>

#include "gentime.h"
#include "timecode.h"
#include "definitions.h"
#include "guide.h"

class Render;
class ClipManager;
class DocClipBase;
class MainWindow;

class KdenliveDoc: public QObject {
Q_OBJECT public:

    KdenliveDoc(const KUrl &url, const KUrl &projectFolder, QUndoGroup *undoGroup, const QString &profileName, const QPoint tracks, MainWindow *parent = 0);
    ~KdenliveDoc();
    QDomNodeList producersList();
    double fps() const;
    int width() const;
    int height() const;
    KUrl url() const;
    KAutoSaveFile *m_autosave;
    void backupMltPlaylist();
    Timecode timecode() const;
    QDomDocument toXml() const;
    void setRenderer(Render *render);
    QUndoStack *commandStack();
    QString producerName(const QString &id);
    void setProducerDuration(const QString &id, int duration);
    int getProducerDuration(const QString &id);
    Render *renderer();
    QDomElement m_guidesXml;
    QDomElement guidesXml() const;
    ClipManager *clipManager();
    void addClip(QDomElement elem, QString clipId, bool createClipItem = true);
    void addClipInfo(QDomElement elem, QString clipId);
    void addFolder(const QString foldername, const QString &clipId, bool edit);
    void deleteFolder(const QString foldername, const QString &clipId);
    void slotAddClipFile(const KUrl url, const QString group, const QString &groupId = QString());
    void slotAddClipList(const KUrl::List urls, const QString group, const QString &groupId = QString());
    void slotAddFolder(const QString folderName);
    void slotDeleteFolder(const QString folderName, const QString &id);
    void slotEditFolder(const QString folderName, const QString oldfolderName, const QString &clipId);
    void slotAddColorClipFile(const QString name, const QString color, QString duration, const QString group, const QString &groupId = QString());
    void slotAddSlideshowClipFile(const QString name, const QString path, int count, const QString duration, const bool loop, const bool fade, const QString &luma_duration, const QString &luma_file, const int softness, const QString group, const QString &groupId = QString());
    void deleteClip(const QString &clipId);
    int getFramePos(QString duration);
    DocClipBase *getBaseClip(const QString &clipId);
    void updateClip(const QString &id);
    void deleteProjectClip(QList <QString> ids);
    void deleteProjectFolder(QMap <QString, QString> map);
    /** Inform application of the audio thumbnails generation progress */
    void setThumbsProgress(const QString &message, int progress);
    QString profilePath() const;
    MltVideoProfile mltProfile() const;
    QString description() const;
    void setUrl(KUrl url);
    QDomElement documentInfoXml();
    void setProfilePath(QString path);
    const QString&getFreeClipId();
    /** does the document need saving */
    bool isModified() const;
    /** Returns project folder, used to store project files (titles, effects,...) */
    KUrl projectFolder() const;
    /** Used to inform main app of the current document loading progress */
    void loadingProgressed();
    void syncGuides(QList <Guide *> guides);
    void setZoom(int factor);
    int zoom() const;
    const double dar();
    double projectDuration() const;
    bool saveSceneList(const QString &path, QDomDocument sceneList);

private:
    KUrl m_url;
    QDomDocument m_document;
    QString m_projectName;
    double m_fps;
    int m_zoom;
    /** Cursor position at document opening */
    int m_startPos;
    int m_width;
    int m_height;
    Timecode m_timecode;
    Render *m_render;
    QUndoStack *m_commandStack;
    QDomDocument generateSceneList();
    ClipManager *m_clipManager;
    MltVideoProfile m_profile;
    QString m_scenelist;
    QTimer *m_autoSaveTimer;
    QString m_searchFolder;
    /** tells whether current doc has been changed since last save event */
    bool m_modified;
    /** Project folder, used to store project files (titles, effects,...) */
    KUrl m_projectFolder;
    double m_documentLoadingStep;
    double m_documentLoadingProgress;
    void convertDocument(double version);
    QDomDocument createEmptyDocument(const int videotracks, const int audiotracks);
    QString colorToString(const QColor& c);
    void checkProjectClips();
    void setNewClipResource(const QString &id, const QString &path);

public slots:
    void slotCreateTextClip(QString group, const QString &groupId);
    /** Set to true if document needs saving, false otherwise */
    void setModified(bool mod = true);

private slots:
    void slotAutoSave();

signals:
    void addProjectClip(DocClipBase *);
    void addProjectFolder(const QString, const QString &, bool, bool edit = false);
    void signalDeleteProjectClip(const QString &);
    void updateClipDisplay(const QString&);
    void deleteTimelineClip(const QString&);
    void progressInfo(const QString &, int);
    /** emited when the document state has been modified (= needs saving or not) */
    void docModified(bool);
    void selectLastAddedClip(const QString &);
    void guidesUpdated();
};

#endif
