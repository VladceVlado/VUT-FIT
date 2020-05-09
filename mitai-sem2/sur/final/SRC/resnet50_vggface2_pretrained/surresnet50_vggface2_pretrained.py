"""
SUR image classifier
Author: Richard Mička
Login: xmicka11
Year: 2020
Polished version of original file available at
https://colab.research.google.com/drive/1Y9TrGEIdcTqO_WNvE0JY4_ekunPekh_L
"""

import sys
import os
import torch
from torch import nn
import torchvision
import PIL
from PIL import Image
import numpy as np
from resnet50_ft_dims_2048 import resnet50_ft
from multiprocessing import freeze_support
freeze_support()

device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")


class SurResNet(nn.Module):
    def __init__(self, resnet_sub_weight_path='senet50_128.pth'):
        super(SurResNet, self).__init__()
        self.submodule = resnet50_ft(weights_path=resnet_sub_weight_path)
        for param in self.submodule.parameters():
            param.requires_grad = False
        # use stride (1,1) instead of (2,2) to reduce the input dimensions
        # form 224x224 to 112x112
        self.submodule.conv1_7x7_s2.stride = (1, 1)
        # fully connected linear layer applied to the resulting feature map of original net
        self.lin1 = nn.Linear(2048, 2)

    def forward(self, in_data):
        with torch.no_grad():
            # class_out is the original vggface2 classifier output which we ignore
            class_out, features = self.submodule.forward(in_data)
        flat_features = features.view(features.size(0), -1)
        o_lin1 = self.lin1(flat_features)
        return o_lin1


def get_new_model(resnet_path):
    return SurResNet(resnet_path)


def load_model(resnet_path, sur_net_path):
    model = SurResNet(resnet_path)
    model.load_state_dict(torch.load(sur_net_path))
    model = model.to(device)
    return model.eval()


def save_model(model, weights_destination):
    torch.save(model.state_dict(), weights_destination)


#
# Datasets and data batch loaders
#
# datadir = '..'
# train_dirs = ['target_train', 'non_target_train']
# dev_dirs = ['target_dev', 'non_target_dev']
# augdev_dirs = ['SURProjekt-master/data_augmentated/data_val/target', 'SURProjekt-master/data_augmentated/data_val/nontarget']
# train_dirs = [os.path.join(datadir, dirname) for dirname in train_dirs]
# dev_dirs = [os.path.join(datadir, dirname) for dirname in dev_dirs]
# augdev_dirs = [os.path.abspath(os.path.join(datadir, dirname)) for dirname in augdev_dirs]
# eval_dir = os.path.join(datadir, 'eval_img')


# data loader from the original network input tweaked to rescale to 3x112x112
def load_data(path='', shape=None):
    mean = (131.0912, 103.8827, 91.4953)
    short_size = 112.0
    crop_size = shape
    img = PIL.Image.open(path)
    im_shape = np.array(img.size)  # in the format of (width, height, *)
    img = img.convert('RGB')

    ratio = float(short_size) / np.min(im_shape)
    img = img.resize(size=(int(np.ceil(im_shape[0] * ratio)),  # width
                           int(np.ceil(im_shape[1] * ratio))),  # height
                     resample=PIL.Image.BILINEAR)

    x = np.array(img)  # image has been transposed into (height, width)
    newshape = x.shape[:2]
    h_start = (newshape[0] - crop_size[0]) // 2
    w_start = (newshape[1] - crop_size[1]) // 2
    x = x[h_start:h_start + crop_size[0], w_start:w_start + crop_size[1]]
    x = x - mean
    return torch.Tensor(x)


class SURDataset(torch.utils.data.Dataset):
    def __init__(self, src_dirs):
        super(SURDataset, self).__init__()

        images = []
        self.labels = []
        self.names = []
        # target
        for filename in os.listdir(src_dirs[0]):
            if not filename.endswith('.png'):
                continue
            img = load_data(os.path.join(src_dirs[0], filename), shape=(112, 112, 3))
            images.append(img)
            self.labels.append(1)
            self.names.append(filename)

        # nontarget
        for filename in os.listdir(src_dirs[1]):
            if not filename.endswith('.png'):
                continue
            img = load_data(os.path.join(src_dirs[1], filename), shape=(112, 112, 3))
            images.append(img)
            self.labels.append(0)
            self.names.append(filename)

        self.images_tensor = torch.stack(images).permute(0, 3, 1, 2)

    def __len__(self):
        return len(self.images_tensor)

    def __getitem__(self, i):
        return self.images_tensor[i, :, :, :], self.labels[i]


class SUREvalDataset(torch.utils.data.Dataset):
    def __init__(self, src_dir):
        super(SUREvalDataset, self).__init__()

        images = []
        self.names = []
        for filename in os.listdir(src_dir):
            if not filename.endswith('.png'):
                continue
            img = load_data(os.path.join(src_dir, filename), shape=(112, 112, 3))
            images.append(img)
            self.names.append(os.path.splitext(filename)[0])

        self.images_tensor = torch.stack(images).permute(0, 3, 1, 2)

    def __len__(self):
        return len(self.images_tensor)

    def __getitem__(self, i):
        return self.images_tensor[i, :, :, :], self.names[i]

batch_size = 4


def load_train_dataset(target_dir, nontarget_dir):
    global train_dataset, train_loader
    train_dataset = SURDataset((target_dir, nontarget_dir))
    train_loader = torch.utils.data.DataLoader(
        train_dataset, batch_size=batch_size, shuffle=True
    )
    print("Train dataset length", len(train_dataset))
    trnData, trnLabels = iter(train_loader).next()
    print("Shapes - data/labels", trnData.shape, trnLabels.shape)


def load_test_dataset(target_dir, nontarget_dir):
    global val_dataset, test_loader
    val_dataset = SURDataset((target_dir, nontarget_dir))
    test_loader = torch.utils.data.DataLoader(
        val_dataset, batch_size=batch_size, shuffle=True
    )
    print("Test dataset length", len(val_dataset))
    testData, testLabels = iter(test_loader).next()
    print("Test shapes - data/labels", testData.shape, testLabels.shape)


def load_aug_test_dataset(target_dir, nontarget_dir):
    global augval_dataset, augtest_loader
    augval_dataset = SURDataset((target_dir, nontarget_dir))
    augtest_loader = torch.utils.data.DataLoader(
        augval_dataset, batch_size=batch_size, shuffle=True
    )
    print("Augmented test dataset length", len(augval_dataset))
    atestData, atestLabels = iter(augtest_loader).next()
    print("Aug. test shapes - data/labels", atestData.shape, atestLabels.shape)


def load_eval_dataset(directory):
    global eval_dataset, eval_loader
    eval_dataset = SUREvalDataset(directory)
    eval_loader = torch.utils.data.DataLoader(
        eval_dataset, batch_size=batch_size, shuffle=False
    )
    print("Evaluation dataset length", len(eval_dataset))
    etestData, etestName = iter(eval_loader).next()
    print("Eval shapes - data/name", etestData.shape, etestName)


#
# Training & Evaluation functions
#
criterion = nn.CrossEntropyLoss()


def test(model, data_loader):
    model.eval()
    loss_acc = 0
    guessed_right = 0
    counter = 0
    samples = 0

    for batch_data, batch_labels in data_loader:
        batch_data = batch_data.to(device)
        batch_labels = batch_labels.to(device)

        logits = model(batch_data)
        loss_acc += criterion(logits, batch_labels).item()

        # compute ratio of correct label predictions
        max_scores, pred_labels = torch.max(logits, dim=1)

        guessed_right += torch.sum(pred_labels == batch_labels).item()

        samples += batch_data.shape[0]
        counter += 1

    model.train()
    return loss_acc / counter, guessed_right / samples, guessed_right, samples


def train(model):
    loss_acc = 0
    guessed_right = 0

    view_step = 50  # every n'th batch gets tested on validation dataset
    optimizer = torch.optim.Adam(model.lin1.parameters(), lr=0.001)
    scheduler = torch.optim.lr_scheduler.StepLR(optimizer, step_size=7, gamma=0.1)

    model = model.train()

    print("Device is:", device)
    model = model.to(device)
    print(next(model.parameters()).is_cuda)
    iteration = 0
    epoch_counter = 0
    try:
        while True:
            for batch_data, batch_labels in train_loader:
                iteration += 1

                batch_data = batch_data.to(device)
                batch_labels = batch_labels.to(device)

                optimizer.zero_grad()

                # forward pass - compute network autput and store all activations
                outputs = model(batch_data)

                # compute loss
                loss = criterion(outputs, batch_labels)
                loss_acc += loss.item()  # .item() disconects from comp. graph

                # Backward pass - compute gradients
                loss.backward()

                # Optimize network
                optimizer.step()

                # compute ratio of correct label predictions
                max_scores, pred_labels = torch.max(outputs, 1)
                # accuracy_acc += torch.sum(pred_labels == batch_labels).item() / float(batch_size)
                guessed_right += torch.sum(pred_labels == batch_labels).item()

                # test network and print some info
                if iteration % view_step == view_step - 1:
                    print('Iteration {0}, Loss: {1:.4f} Accuracy: {2:.4f} ({3}/{4})'
                          .format(iteration, loss_acc / view_step, guessed_right / (view_step * batch_size), guessed_right, view_step * batch_size))

                    test_loss_acc, test_accuracy_acc, test_guessed_right, test_guesses_total = test(model, test_loader)
                    print('Test, Loss: {:.4f} Accuracy: {:.4f} ({}/{})'
                          .format(test_loss_acc, test_accuracy_acc, test_guessed_right, test_guesses_total))
                    loss_acc = 0
                    guessed_right = 0
            epoch_counter += 1
            print(f"end of epoch {epoch_counter}")
            scheduler.step()
    except KeyboardInterrupt:
        print("trainig interrupted, save the weights using save_model(model, weights_destination)")


def evaluate_dataset(model):
    result_dict = dict()
    model.eval()
    for samples, filenames in eval_loader:
        samples = samples.to(device)

        model_out = model(samples)
        max_scores, pred_labels = torch.max(model_out, dim=1)
        probabilities = torch.nn.functional.softmax(model_out, dim=1)

        for sample_i in range(len(filenames)):
            result_dict[filenames[sample_i]] = (model_out[sample_i], probabilities[sample_i], pred_labels[sample_i])
    return result_dict


def dump_result_dict(result_dict, path):
    f = None
    dumpfile = True if path else False
    try:
        if dumpfile:
            f = open(path, 'w')
            for filename in sorted(result_dict.keys()):
                predicted = result_dict[filename]
                result = f"{filename}\t{predicted[1][1]:.4f}\t{predicted[2]}"
                print(result)
                if dumpfile:
                    f.write(result + "\n")
    finally:
        if f is not None:
            f.close()
