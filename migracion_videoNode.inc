<?php

class nnmxNodeVideoMigration extends Migration {
  public function __construct() {
    parent::__construct();
    // Human-friendly description of your migration process. Be as detailed as you like.
    $this->description = t('Migrate nnmx videos.');

    $this->dependencies = array('nnmxTermEdicion', 'nnmxTermUbicacion', 'nnmxUser', 'nnmxFileTable');


    // Create a map object for tracking the relationships between source rows
    // and their resulting Drupal objects to make posible rollback

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'nid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'D6 Unique Node ID',
          'alias' => 'n',
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );


    // We are getting data from tables in the Drupal default database - first,
    // set up a query for this data.
    $query = db_select(SOURCE_DATABASE . '.node', 'n')
      ->fields('n', array('nid', 'vid', 'type', 'language', 'title', 'uid', 'status', 'created', 'changed', 'comment', 'promote', 'moderate', 'sticky', 'tnid', 'translate'))
      ->condition('n.type', 'video', '=');
    $query->join(SOURCE_DATABASE . '.node_revisions', 'nr', 'n.vid = nr.vid');
    $query->addField('nr', 'body');
    $query->addField('nr', 'teaser');
    $query->addField('nr', 'format');
    $query->join(SOURCE_DATABASE . '.users', 'u', 'n.uid = u.uid');
    $query->addField('u', 'name');
    $query->orderBy('n.nid', 'ASC');

    // TIP: By passing an array of source fields to the MigrateSourceSQL constructor,
    // we can modify the descriptions of source fields (which just default, for
    // SQL migrations, to table_alias.column_name), as well as add additional fields
    // (which may be populated in prepareRow()).
    $source_fields = array(
      'nid' => t('The node ID of the page'),
      'url_alias' => t('The url alias of the node'),
      'edicion' => t('The category for the node'),
      'ubicacion' => t('The category for the node'),
      'content_field_autor' => t('author of video'),
    );

    // Create a MigrateSource object, which manages retrieving the input data.
    $this->source = new MigrateSourceSQL($query, $source_fields);

    // Set up our destination - node in this case.
    $this->destination = new MigrateDestinationNode('video');

// Assign mappings TO destination fields FROM source fields.
    $this->addFieldMapping('is_new')->defaultValue(TRUE);
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('nid', 'nid');
    $this->addFieldMapping('uid', 'uid');
    $this->addFieldMapping('revision')->defaultValue(TRUE);
    $this->addFieldMapping('revision_uid', 'uid');
    $this->addFieldMapping('created', 'created');
    $this->addFieldMapping('changed', 'changed');
    $this->addFieldMapping('status', 'status');
    $this->addFieldMapping('promote', 'promote');
    $this->addFieldMapping('sticky', 'sticky');
    $this->addFieldMapping('comment', 'comment');
    $this->addFieldMapping('path', 'url_alias');
    $this->addFieldMapping('language')->defaultValue(LANGUAGE_NONE);
    $this->addFieldMapping('field_edicion', 'edicion')->separator(',');
    $this->addFieldMapping('field_ubicacion', 'ubicacion')->separator(',');
    $body_arguments = MigrateTextFieldHandler::arguments(array('source_field' => 'teaser'), array('source_field' => 'format'));
    $this->addFieldMapping('body', 'body')->arguments($body_arguments);
    $this->addFieldMapping('field_autores', 'content_field_autor');

  // Unmapped source fields
    $this->addUnmigratedSources(array('vid', 'type', 'language', 'moderate', 'tnid', 'translate', 'teaser', 'format', 'name'));
  }

    public function prepareRow($current_row) {
    // Set the uid for the node revision.
    //$current_row->uid = $current_row->revision_uid = xdeb_migration_get_user($current_row->name);

    // Set the text format for the node.
    $current_row->format = nnmx_migration_get_text_format($current_row->format);

    // Set the terms for the node.
    $current_row->edicion = nnmx_migration_get_terms($current_row->vid, NNMX_EDICION_VID);
    $current_row->ubicacion = nnmx_migration_get_terms($current_row->vid, NNMX_UBICACION_VID);

    // Set the url alias for the node.
    $current_row->url_alias = nnmx_migration_get_url_alias($current_row->nid);

    $current_row->content_field_autor = nnmx_migration_get_textfield($current_row->vid, 'content_field_autor', 'field_autor_value');


    // We could also have used this function to decide to skip a row, in cases
    // where that couldn't easily be done through the original query. Simply
    // return FALSE in such cases.
    return TRUE;
  }


}
