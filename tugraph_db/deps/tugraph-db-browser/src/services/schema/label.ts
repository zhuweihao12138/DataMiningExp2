import { DATA_TYPE } from '@/constants';
import {
  alterLabelAddFields,
  alterLabelDelFields,
  alterLabelModFields,
} from '@/queries/schema';
import { IDeleteSchemaParams, IUpdateSchemaParams } from '@/types/services';
import { responseFormatter } from '@/utils/schema';
import { Driver } from 'neo4j-driver';
import { request } from '../request';

/**
 * 删除 指定 Label 中的属性字段
 * @param params
 * @returns
 */
export const deleteLabelField = async (
  driver: Driver,
  params: IDeleteSchemaParams,
) => {
  const { graphName, labelType, labelName, propertyNames } = params;

  try {
    const type = labelType === 'node' ? 'vertex' : 'edge';
    const cypher = alterLabelDelFields(
      type,
      labelName,
      JSON.stringify(propertyNames),
    );

    const result = await request({ driver, cypher, graphName });
    return responseFormatter(result);
  } catch (error) {
    console.log(error);
  }
};

/**
 * 向指定的 label 中添加属性
 * @param params
 * @returns
 */
export const addFieldToLabel = async (
  driver: Driver,
  params: IUpdateSchemaParams,
) => {
  const { graphName, labelType, labelName, properties } = params;

  let condition = '';
  properties.forEach((d, index) => {
    const { name, type, optional = false } = d;
    const currentType = DATA_TYPE.find(item => item['value'] === type);
    const isINT = `${type}`.includes('INT');

    const isBOOL = `${type}`.includes('BOOL');
    const isDOUBLE = `${type}`.includes('DOUBLE');
    const defaultValue =
      isINT || isBOOL || isDOUBLE
        ? currentType?.default
        : `'${currentType?.default}'`;

    const ending = index === properties.length - 1 ? '' : ',';

    condition += `['${name}', '${type}', ${defaultValue}, ${optional}]${ending}`;
  });

  const type = labelType === 'node' ? 'vertex' : 'edge';

  const cypher = alterLabelAddFields(type, labelName, condition);

  const result = await request({ driver, cypher, graphName });
  return responseFormatter(result);
};

/**
 * 修改 Label 中指定的属性字段
 * @param params
 * @returns
 */
export const updateFieldToLabel = async (
  driver: Driver,
  params: IUpdateSchemaParams,
) => {
  const { graphName, labelType, labelName, properties } = params;

  let condition = '';
  properties.forEach((d, index) => {
    const { name, type, optional = false } = d;

    const ending = index === properties.length - 1 ? '' : ',';

    condition += `['${name}', '${type}', ${optional} ]${ending}`;
  });

  const type = labelType === 'node' ? 'vertex' : 'edge';

  let cypher = alterLabelModFields(type, labelName, condition);
  const result = await request({ driver, cypher, graphName });
  return responseFormatter(result);
};
