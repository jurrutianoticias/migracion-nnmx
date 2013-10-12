<?php

class nnmxYoutubeRevisionTableMigration extends Migration {
  public function __construct() {
    parent::__construct();
    // Human-friendly description of your migration process. Be as detailed as you like.
    $this->description = t('Migrate table Files');

    $this->dependencies = array('nnmxNodeBlog', 'nnmxNodeVideo');

    $table_name = 'field_revision_field_youtube';
    $this->map = new MigrateSQLMap($this->machineName,
        array('vid' => array(
              'type' => 'int',
              'unsigned' => TRUE,
              'not null' => TRUE,
              'description' => 'table vid',
              'alias' => 'n',
            )
          ),
        MigrateDestinationTable::getKeySchema($table_name)
      );


//query
$query = db_select(SOURCE_DATABASE . '.node', 'n')
      ->fields('n', array('nid', 'vid', 'type', 'language', 'title', 'uid', 'status', 'created', 'changed', 'comment', 'promote', 'moderate', 'sticky', 'tnid', 'translate'))
      ->condition(db_or()->condition('n.type', 'video', '=')->condition('n.type', 'blog', '='));
    $query->join(SOURCE_DATABASE . '.content_field_video_youtube', 'nr', 'n.vid = nr.vid');
    $query->addField('nr', 'field_video_youtube_value');

    //source fields

    $source_fields = array(
      'link_youtube' => t('link from youtube'),
    );

    $this->source = new MigrateSourceSQL($query, $source_fields);
    $this->destination = new MigrateDestinationTable($table_name);


//map fields
        //fid uid filename URI  filemime  filesize status timestamp type
  $this->addFieldMapping('entity_type')->defaultValue('node');
  $this->addFieldMapping('bundle', 'type');
  $this->addFieldMapping('deleted')->defaultValue(0);
  $this->addFieldMapping('entity_id', 'nid');
  $this->addFieldMapping('revision_id', 'vid');
  $this->addFieldMapping('language')->defaultValue('und');
  $this->addFieldMapping('delta')->defaultValue(0);
  $this->addFieldMapping('field_youtube_input', 'link_youtube');
  $this->addFieldMapping('field_youtube_video_id', 'field_video_youtube_value');
  }

   public function prepareRow($current_row) {
    $current_row->link_youtube = 'http://www.youtube.com/watch?v=' . $current_row->field_video_youtube_value;
   }
}
