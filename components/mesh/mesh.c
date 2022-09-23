//
// Created by jadjer on 14.09.22.
//

#include "mesh.h"
#include "bluetooth.h"
#include "sensors.h"
#include <esp_ble_mesh_common_api.h>
#include <esp_ble_mesh_config_model_api.h>
#include <esp_ble_mesh_networking_api.h>
#include <esp_ble_mesh_provisioning_api.h>
#include <esp_ble_mesh_generic_model_api.h>
#include <string.h>

#define TAG "Bluetooth Mesh"

#define CID_ESP 0x02E5

static uint8_t devUuid[ESP_BLE_MESH_OCTET16_LEN] = {0x32, 0x10};

static esp_ble_mesh_cfg_srv_t configServer = {
    .relay = ESP_BLE_MESH_RELAY_ENABLED,
    .beacon = ESP_BLE_MESH_BEACON_ENABLED,
    .friend_state = ESP_BLE_MESH_FRIEND_ENABLED,
    .gatt_proxy = ESP_BLE_MESH_GATT_PROXY_ENABLED,
    .default_ttl = 7,
    .net_transmit = ESP_BLE_MESH_TRANSMIT(2, 20),
    .relay_retransmit = ESP_BLE_MESH_TRANSMIT(2, 20),
};

ESP_BLE_MESH_MODEL_PUB_DEFINE(onoff_pub_0, 2 + 3, ROLE_NODE);
static esp_ble_mesh_gen_onoff_srv_t onoff_server_0 = {
    .rsp_ctrl.get_auto_rsp = ESP_BLE_MESH_SERVER_AUTO_RSP,
    .rsp_ctrl.set_auto_rsp = ESP_BLE_MESH_SERVER_AUTO_RSP,
};

ESP_BLE_MESH_MODEL_PUB_DEFINE(battery_pub_0, 2 + 3, ROLE_NODE);
static esp_ble_mesh_gen_battery_srv_t battery_server_0 = {
    .rsp_ctrl.get_auto_rsp = ESP_BLE_MESH_SERVER_AUTO_RSP,
    .rsp_ctrl.set_auto_rsp = ESP_BLE_MESH_SERVER_AUTO_RSP,
};

ESP_BLE_MESH_MODEL_PUB_DEFINE(rpm_pub_0, 2 + 3, ROLE_NODE);
static esp_ble_mesh_gen_level_srv_t rpm_server_0 = {
    .rsp_ctrl.get_auto_rsp = ESP_BLE_MESH_SERVER_AUTO_RSP,
    .rsp_ctrl.set_auto_rsp = ESP_BLE_MESH_SERVER_AUTO_RSP,
};

static esp_ble_mesh_model_t rootModels[] = {
    ESP_BLE_MESH_MODEL_CFG_SRV(&configServer),
    ESP_BLE_MESH_MODEL_GEN_ONOFF_SRV(&onoff_pub_0, &onoff_server_0),
    ESP_BLE_MESH_MODEL_GEN_BATTERY_SRV(&battery_pub_0, &battery_server_0),
    ESP_BLE_MESH_MODEL_GEN_LEVEL_SRV(&rpm_pub_0, &rpm_server_0)
};

static esp_ble_mesh_elem_t elements[] = {
    ESP_BLE_MESH_ELEMENT(0, rootModels, ESP_BLE_MESH_MODEL_NONE),
//    ESP_BLE_MESH_ELEMENT(0, extend_model_0, ESP_BLE_MESH_MODEL_NONE),
//    ESP_BLE_MESH_ELEMENT(0, extend_model_1, ESP_BLE_MESH_MODEL_NONE),
};

static esp_ble_mesh_comp_t composition = {
    .cid = CID_ESP,
    .elements = elements,
    .element_count = ARRAY_SIZE(elements),
};

static esp_ble_mesh_prov_t provision = {
    .uuid = devUuid,
};

void provisionComplete(uint16_t net_idx, uint16_t addr, uint8_t flags, uint32_t iv_index) {
  ESP_LOGI(TAG, "net_idx 0x%03x, addr 0x%04x", net_idx, addr);
  ESP_LOGI(TAG, "flags 0x%02x, iv_index 0x%08x", flags, iv_index);
  //  board_led_operation(LED_G, LED_OFF);

  VehicleData_t vehicleData;
  vehicleData.speed = 123;

  sensorsUpdateVehicleData(&vehicleData);

  EngineData_t engineData;
  engineData.rpm = 4521;

  sensorsUpdateEngineData(&engineData);

  SensorsData_t sensorsData;
  sensorsData.tpsPercent = 38;

  sensorsUpdateSensorsData(&sensorsData);
}

void bluetoothMeshProvisioningCallback(esp_ble_mesh_prov_cb_event_t event, esp_ble_mesh_prov_cb_param_t* param) {
  switch (event) {
    case ESP_BLE_MESH_PROV_REGISTER_COMP_EVT:
      ESP_LOGI(TAG, "ESP_BLE_MESH_PROV_REGISTER_COMP_EVT, err_code %d", param->prov_register_comp.err_code);
      break;
    case ESP_BLE_MESH_NODE_PROV_ENABLE_COMP_EVT:
      ESP_LOGI(TAG, "ESP_BLE_MESH_NODE_PROV_ENABLE_COMP_EVT, err_code %d", param->node_prov_enable_comp.err_code);
      break;
    case ESP_BLE_MESH_NODE_PROV_LINK_OPEN_EVT:
      ESP_LOGI(TAG, "ESP_BLE_MESH_NODE_PROV_LINK_OPEN_EVT, bearer %s",
               param->node_prov_link_open.bearer == ESP_BLE_MESH_PROV_ADV ? "PB-ADV" : "PB-GATT");
      break;
    case ESP_BLE_MESH_NODE_PROV_LINK_CLOSE_EVT:
      ESP_LOGI(TAG, "ESP_BLE_MESH_NODE_PROV_LINK_CLOSE_EVT, bearer %s",
               param->node_prov_link_close.bearer == ESP_BLE_MESH_PROV_ADV ? "PB-ADV" : "PB-GATT");
      break;
    case ESP_BLE_MESH_NODE_PROV_COMPLETE_EVT:
      ESP_LOGI(TAG, "ESP_BLE_MESH_NODE_PROV_COMPLETE_EVT");
      provisionComplete(param->node_prov_complete.net_idx, param->node_prov_complete.addr,
                        param->node_prov_complete.flags, param->node_prov_complete.iv_index);
      break;
    case ESP_BLE_MESH_NODE_PROV_RESET_EVT:
      ESP_LOGI(TAG, "ESP_BLE_MESH_NODE_PROV_RESET_EVT");
      break;
    case ESP_BLE_MESH_NODE_SET_UNPROV_DEV_NAME_COMP_EVT:
      ESP_LOGI(TAG, "ESP_BLE_MESH_NODE_SET_UNPROV_DEV_NAME_COMP_EVT, err_code %d", param->node_set_unprov_dev_name_comp.err_code);
      break;
    default:
      break;
  }
}

void bluetoothMeshConfigServerCallback(esp_ble_mesh_cfg_server_cb_event_t event, esp_ble_mesh_cfg_server_cb_param_t* param) {
  if (event != ESP_BLE_MESH_CFG_SERVER_STATE_CHANGE_EVT) {
    return;
  }

  switch (param->ctx.recv_op) {
    case ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD:
      ESP_LOGI(TAG, "ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD");
      ESP_LOGI(TAG, "net_idx 0x%04x, app_idx 0x%04x",
               param->value.state_change.appkey_add.net_idx,
               param->value.state_change.appkey_add.app_idx);
      ESP_LOG_BUFFER_HEX("AppKey", param->value.state_change.appkey_add.app_key, 16);
      break;
    case ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND:
      ESP_LOGI(TAG, "ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND");
      ESP_LOGI(TAG, "elem_addr 0x%04x, app_idx 0x%04x, cid 0x%04x, mod_id 0x%04x",
               param->value.state_change.mod_app_bind.element_addr,
               param->value.state_change.mod_app_bind.app_idx,
               param->value.state_change.mod_app_bind.company_id,
               param->value.state_change.mod_app_bind.model_id);
      break;
    case ESP_BLE_MESH_MODEL_OP_MODEL_SUB_ADD:
      ESP_LOGI(TAG, "ESP_BLE_MESH_MODEL_OP_MODEL_SUB_ADD");
      ESP_LOGI(TAG, "elem_addr 0x%04x, sub_addr 0x%04x, cid 0x%04x, mod_id 0x%04x",
               param->value.state_change.mod_sub_add.element_addr,
               param->value.state_change.mod_sub_add.sub_addr,
               param->value.state_change.mod_sub_add.company_id,
               param->value.state_change.mod_sub_add.model_id);
      break;
    default:
      break;
  }
}

void bluetoothMeshGenericServeCallback(esp_ble_mesh_generic_server_cb_event_t event, esp_ble_mesh_generic_server_cb_param_t* param) {
  esp_ble_mesh_gen_onoff_srv_t* srv;
  ESP_LOGI(TAG, "event 0x%02x, opcode 0x%04x, src 0x%04x, dst 0x%04x", event, param->ctx.recv_op, param->ctx.addr, param->ctx.recv_dst);

  switch (event) {
    case ESP_BLE_MESH_GENERIC_SERVER_STATE_CHANGE_EVT:
      ESP_LOGI(TAG, "ESP_BLE_MESH_GENERIC_SERVER_STATE_CHANGE_EVT");
      if (param->ctx.recv_op == ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET || param->ctx.recv_op == ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK) {
        ESP_LOGI(TAG, "onoff 0x%02x", param->value.state_change.onoff_set.onoff);
        //        example_change_led_state(param->model, &param->ctx, param->value.state_change.onoff_set.onoff);
      }
      break;
    case ESP_BLE_MESH_GENERIC_SERVER_RECV_GET_MSG_EVT:
      ESP_LOGI(TAG, "ESP_BLE_MESH_GENERIC_SERVER_RECV_GET_MSG_EVT");
      if (param->ctx.recv_op == ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_GET) {
        srv = param->model->user_data;
        ESP_LOGI(TAG, "onoff 0x%02x", srv->state.onoff);
        //        example_handle_gen_onoff_msg(param->model, &param->ctx, NULL);
      }
      break;
    case ESP_BLE_MESH_GENERIC_SERVER_RECV_SET_MSG_EVT:
      ESP_LOGI(TAG, "ESP_BLE_MESH_GENERIC_SERVER_RECV_SET_MSG_EVT");
      if (param->ctx.recv_op == ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET || param->ctx.recv_op == ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK) {
        ESP_LOGI(TAG, "onoff 0x%02x, tid 0x%02x", param->value.set.onoff.onoff, param->value.set.onoff.tid);
        if (param->value.set.onoff.op_en) {
          ESP_LOGI(TAG, "trans_time 0x%02x, delay 0x%02x",
                   param->value.set.onoff.trans_time, param->value.set.onoff.delay);
        }
        //        example_handle_gen_onoff_msg(param->model, &param->ctx, &param->value.set.onoff);
      }
      break;
    default:
      ESP_LOGE(TAG, "Unknown Generic Server event 0x%02x", event);
      break;
  }
}

esp_err_t bluetoothMeshInit() {
  esp_err_t err;

  bluetoothGetDeviceUUID(devUuid);

  esp_ble_mesh_register_prov_callback(bluetoothMeshProvisioningCallback);
  esp_ble_mesh_register_config_server_callback(bluetoothMeshConfigServerCallback);
  esp_ble_mesh_register_generic_server_callback(bluetoothMeshGenericServeCallback);

  err = esp_ble_mesh_init(&provision, &composition);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to initialize mesh stack");
    return err;
  }

  err = esp_ble_mesh_node_prov_enable(ESP_BLE_MESH_PROV_ADV | ESP_BLE_MESH_PROV_GATT);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to enable mesh node");
    return err;
  }

  //  board_led_operation(LED_G, LED_ON);

  ESP_LOGI(TAG, "BLE Mesh sensor server initialized");

  return ESP_OK;
}
